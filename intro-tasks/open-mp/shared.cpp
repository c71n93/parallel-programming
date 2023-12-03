#include <omp.h>
#include <iostream>

int main() {
    omp_set_num_threads(8);
    int shared = 0;

#pragma omp parallel shared(shared)
{
    size_t tid = omp_get_thread_num();
    while (shared != tid) {}
    #pragma omp critical
    {
        shared++;
        std::cout << "[Thread " << tid << "] " << "new value: " << shared << std::endl;
    }
}

}