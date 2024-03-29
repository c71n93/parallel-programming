#include <cstdio>
#include <string>
#include <omp.h>
#include <cmath>
#include <cstring>

constexpr int SIZE = 4096;
constexpr int YSIZE = SIZE;
constexpr int XSIZE = SIZE;

double **empty_matrix(int len_y, int len_x);
void free_matrix(double **matrix, int len_y);
static inline void fill_vector(int start_y, int start_x, double** matrix) {
    for (int y = start_y, x = start_x; x >= 3 && y < YSIZE - 2; x -= 3, y += 2) {
        matrix[y][x] = sin(0.1 * matrix[y+2][x-3]);
    }
}

int main (int argc, char **argv) {
    std::string out = "result_parallel.csv";
    if (argc == 3) {
        out = argv[2];
    }
    double** matrix = empty_matrix(YSIZE, XSIZE);
    FILE *ff;
    for (int y=0; y < YSIZE; y++){
        for (int x=0; x < XSIZE; x++){
            matrix[y][x] = 10 * y + x;
        }
    }

//начало измерения времени
    double time_1 = omp_get_wtime();
    const int threads = strtol(argv[1], nullptr, 10);
    if (threads <= 1) {
        for (int y=0; y < YSIZE - 1; y++){
            for (int x = 1; x < XSIZE; x++){
                matrix[y][x] = sin(0.1 * matrix[y+1][x-1]);
            }
        }
        goto Finish;
    }
    omp_set_num_threads(threads);
#pragma omp parallel
    {
        int curr_thr = omp_get_thread_num();
        int lines = YSIZE / threads;
        int start = lines * curr_thr;
        double saved[XSIZE];
        if (curr_thr != threads - 1) {
            std::memcpy(saved, reinterpret_cast<const void *>(matrix[start + lines]),
                        XSIZE * sizeof(double));
        }
//        printf("[thread %i]: start with %i, save line %i\n", curr_thr, start, start + lines);
#pragma omp barrier
        int y;
        for (y = start; y < start + lines - 1; y += 1) {
            for (int x = 1; x < XSIZE; ++x) {
                matrix[y][x] = sin(0.1 * matrix[y+1][x-1]);
            }
        }
        if (curr_thr != threads - 1) {
            for (int x = 1; x < XSIZE; ++x) {
                matrix[y][x] = sin(0.1 * saved[x - 1]);
            }
        }
//        printf("thr %i => %i\n", curr_thr, curr_thr / 2);
    }
    Finish:
    double time_2 = omp_get_wtime();
//окончание измерения времени
    printf("%lf", time_2 - time_1);
    ff = fopen(out.c_str(),"w");
    for(int y= 0; y < YSIZE; y++){
        for (int x= 0; x < XSIZE; x++){
            fprintf(ff, "%f ", matrix[y][x]);
            if (x < XSIZE - 1) {
                fprintf(ff,", ");
            }
        }
        fprintf(ff, "\n");
    }
    free_matrix(matrix, YSIZE);
    fclose(ff);

}

double **empty_matrix(int len_y, int len_x) {
    double **res = (double **) calloc(len_y, sizeof(double *));
    for (int i = 0; i < len_y; ++i) {
        res[i] = (double *) calloc(len_x, sizeof(double));
    }
    return res;
}

void free_matrix(double **matrix, int len_y) {
    for (int t = 0; t < len_y; ++t) {
        free(matrix[t]);
    }
    free(matrix);
}
