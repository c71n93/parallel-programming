#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <cmath>
#include <cstring>

#define X 1.0
#define T 1.0
#define NOT_SEND -1

#define HALF_CELL 10

int len_x;
int len_t;
double h;
double tau;
double beta;
double alpha = 0.5;

double phi(double x) {
    return exp(-x);
}
double psi(double t) {
    return cos(M_PI * t);
}
double f(double  x, double t) {
    return x * t;
}

double **empty_matrix(int len_x, int len_t) {
    double **res = (double **) calloc(len_t, sizeof(double *));
    for (int i = 0; i < len_t; ++i) {
        res[i] = (double *) calloc(len_x, sizeof(double));
    }
    return res;
}
void free_matrix(double **matrix, int len_t) {
    for (int t = 0; t < len_t; ++t) {
        free(matrix[t]);
    }
    free(matrix);
}
void init_fragment(double **empty, int  start_x, int finish_x, int len_t) {
    if (start_x == 0) {
        for (int t = 0; t < len_t; ++t) {
            empty[t][0] = psi(tau * t);
        }
    }
    for (int x = 0; x < finish_x - start_x; ++x) {
        empty[0][x] = phi(h * (x + start_x));
    }
}
double inline static compute_point(double eq_eq, double eq_plus, double plus_eq, int x, int t) {
    double res = (2 * f(tau * (t), h * (x)) * tau - (plus_eq - eq_eq - eq_plus) - beta * alpha * (eq_plus - eq_eq - plus_eq)) / (1 + beta * alpha);
    return res;
}
void compute_section(double **section, int start_x, int finish_x, int next_rank) {
    if (start_x == 0) {
        for (int t = 0; t < len_t; ++t) {
            for (int x = 0; x < finish_x - start_x; ++x) {
                if (x && t) {
                    section[t][x] = compute_point(
                            section[t - 1][x - 1],
                            section[t - 1][x],
                            section[t][x - 1],
                            start_x + x,
                            t
                    );
                }
            }
            if (next_rank != NOT_SEND) {
                MPI_Send(
                        &section[t][finish_x - start_x - 1],
                        1,
                        MPI_DOUBLE,
                        next_rank,
                        0,
                        MPI_COMM_WORLD
                );
            }
        }
    } else {
        double *prev_col = (double *) calloc(len_t, sizeof(double));
        for (int t = 0; t < len_t; ++t) {
            MPI_Status status;
            MPI_Recv(
                    prev_col + t,
                    1,
                    MPI_DOUBLE,
                    MPI_ANY_SOURCE,
                    MPI_ANY_TAG,
                    MPI_COMM_WORLD,
                    &status
            );
            for (int x = 0; x < finish_x - start_x; ++x) {
                if (t == 0) {
                    ;
                } else {
                    double eq_eq, eq_plus, plus_eq;
                    eq_plus = section[t-1][x];
                    if (x == 0) {
                        eq_eq = prev_col[t - 1];
                        plus_eq = prev_col[t];
                    } else {
                        eq_eq = section[t-1][x-1];
                        plus_eq = section[t][x-1];
                    }
                    section[t][x] = compute_point(eq_eq, eq_plus, plus_eq, start_x + x, t);
                }
            }
            if (next_rank != NOT_SEND) {
                MPI_Send(
                        &section[t][finish_x - start_x - 1],
                        1,
                        MPI_DOUBLE,
                        next_rank,
                        0,
                        MPI_COMM_WORLD
                );
            }
        }
        free(prev_col);
    }
}

void save(char* file, double **section, int start_x, int finish_x) {
    MPI_File_delete(file, MPI_INFO_NULL);
    MPI_File handle;
    MPI_File_open(
            MPI_COMM_WORLD,
            file,
            MPI_MODE_RDWR | MPI_MODE_CREATE,
            MPI_INFO_NULL,
            &handle
    );
    for (int t = 0; t < len_t; ++t) {
        int offset = 2 * HALF_CELL * (len_x * t + start_x);
        char* line = (char *) calloc(2 * HALF_CELL * (finish_x - start_x), 1);
        for (int x = 0; x < finish_x - start_x; ++x) {
            char number[25] = {' '};
            number[20] = 0;
            sprintf(number, "%.10lf", section[t][x]);
            for (int i = 0; i < 25; ++i) {
                char c = number[i];
                if (c == 0) {
                    number[i] = ' ';
                }
            }
            int last_in_line = (x + start_x == len_x - 1);
            number[17] = last_in_line ? ' ': ',';
            number[19] = last_in_line ? '\n' : ' ';
            memcpy(line + x * 20, number, 20);
        }
        MPI_Status status;
        MPI_File_write_at(
                handle,
                offset,
                line,
                2 * HALF_CELL * (finish_x - start_x),
                MPI_CHAR,
                &status
        );
        free(line);
    }
    MPI_File_close(&handle);
}

int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 4) {
        printf("Wrong number of args! Please provide 2 arguments\n");
        exit(-1);
    }
    char* file;
    if (argc == 4) {
        file = argv[3];
    } else {
        file = "solution.csv";
    }
    len_x = (int) strtol(argv[1], NULL, 10);
    len_t = (int) strtol(argv[2], NULL, 10);
    h = X / (len_x - 1);
    tau = T / (len_t - 1);
    beta = tau / h;
    MPI_Init(&argc, &argv);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Barrier(MPI_COMM_WORLD);
    double time_1 = MPI_Wtime();
    int start_x = my_rank * (len_x / world_size);
    int next_start = (my_rank + 1) * (len_x / world_size);
    int finish_x = (my_rank == world_size - 1) ? len_x : next_start;
    double **fragment = empty_matrix(finish_x - start_x, len_t);
    init_fragment(fragment, start_x, finish_x, len_t);
    compute_section(
            fragment,
            start_x,
            finish_x,
            (my_rank + 1 == world_size) ? NOT_SEND: my_rank + 1
    );
    MPI_Barrier(MPI_COMM_WORLD);
    double time_2 = MPI_Wtime();
    if (my_rank == 0) {
        printf("%lf", time_2 - time_1);
    }
    save(file, fragment, start_x, finish_x);
    free_matrix(fragment, len_t);
    MPI_Finalize();
    return EXIT_SUCCESS;
}