#include <omp.h>
#include <iostream>

int main () {
    omp_set_num_threads(8);

#pragma omp parallel
    printf("thread %d/%d: Hello World\n", omp_get_thread_num(), omp_get_max_threads());
    return 0;
}
