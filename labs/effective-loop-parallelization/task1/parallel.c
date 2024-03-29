#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define SIZE 5000

#define YSIZE SIZE
#define XSIZE SIZE

int my_rank;

static inline double initial_value(int i, int j) {
    return 10 * i + j;
}

double **init_matrix(int len_x, int len_y) {
    double **res = (double **) calloc(len_x, sizeof(double *));
    for (int i = 0; i < len_x; ++i) {
        res[i] = (double *) calloc(len_y, sizeof(double));
    }
    return res;
}

void free_matrix(double **matrix, int len) {
    for (int t = 0; t < len; ++t) {
        free(matrix[t]);
    }
    free(matrix);
}

void initMatrix(double** matrix) {
    for (int i=0; i < YSIZE; i++) {
        for (int j=0; j < XSIZE; j++) {
            matrix[j][i] = 10 * i + j;
        }
    }
}

void foldMatrixByVector(double **matrix, const double *vector, const int len) {
    int x0 = (int) vector[0];
    for (int y = 0, i = 1; i < len; ++i, ++y) {
        matrix[x0][y] = vector[i];
    }
}

void computeRows(int x_root) {
    double message[YSIZE + 1];
    int x0 = x_root;
    message[0] = (double) x0;
    double *arr = message + 1;
    for (int y = 0; y < YSIZE; ++y) {
        arr[y] = initial_value(y, x0);
    }
    MPI_Send(
        message,
        YSIZE + 1,
        MPI_DOUBLE,
        0, // the zero rank.
        0,
        MPI_COMM_WORLD
    );
    for (x0 = x0 + 8; x0 < XSIZE; x0 += 8) {
        message[0] = (double) x0;
        for (int y = YSIZE-1; y > 0; --y) {
            arr[y] = sin(5 * arr[y - 1]);
        }
        arr[0] = initial_value(0, x0);
        MPI_Send(
            message,
            YSIZE + 1,
            MPI_DOUBLE,
            0, // the zero rank.
            0,
            MPI_COMM_WORLD
        );
    }
}

int main(int argc, char* argv[]) {
    if (argc != 1 && argc != 2) {
        printf("Wrong number of args! Please provide 2 arguments\n");
        exit(-1);
    }
    char* file;
    if (argc == 2) {
        file = argv[3];
    } else {
        file = "../result_parallel.csv";
    }
    MPI_Init(&argc, &argv);
    double** matrix = NULL;
    int world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size == 1) {
        printf("world_size == 1");
        abort();
    }

    if(my_rank == 0) {
        matrix = init_matrix(XSIZE, YSIZE);
        initMatrix(matrix);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double time_1 = MPI_Wtime();
    if (my_rank == 0) {
        double buf[YSIZE + 1];
        int messages = YSIZE;
        MPI_Status status;
        for (int i = 0; i < messages; ++i) {
            MPI_Recv(
                buf,
                YSIZE + 1,
                MPI_DOUBLE,
                MPI_ANY_SOURCE,
                MPI_ANY_SOURCE,
                MPI_COMM_WORLD,
                &status
            );
            int message_len = 0;
            MPI_Get_count(&status, MPI_DOUBLE, &message_len);
            foldMatrixByVector(matrix, buf, message_len);}
    } else {
        // Not null rank
        int s = world_size - 1; // executing processes
        // Computing row roots
        int root = my_rank - 1;
        while (root < 8) {
            computeRows(root);
            root += s;
        }
    }
    double time_2 = MPI_Wtime();
    if (my_rank == 0) {
        printf("%lf", time_2 - time_1);
        FILE *ff;
        ff = fopen(file,"w");
        for(int y= 0; y < YSIZE; y++){
            for (int x= 0; x < XSIZE; x++){
                fprintf(ff, "%f ", matrix[x][y]);
                if (x < XSIZE - 1) {
                    fprintf(ff,", ");
                }
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
        free_matrix(matrix, YSIZE);
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}

