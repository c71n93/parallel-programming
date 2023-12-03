#include <omp.h>
#include <stdexcept>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 3) {
        throw std::invalid_argument("Invalid number of arguments. Number of threads is required.");
    }
    const size_t num_threads = static_cast<size_t>(std::strtol(argv[1], nullptr, 10));
    const size_t N = static_cast<size_t>(std::strtol(argv[2], nullptr, 10));

    omp_set_num_threads(num_threads);

    double sum = 0.0;
    double start = omp_get_wtime();
#pragma omp parallel for schedule(auto) reduction(+ : sum) shared(N)
    for (int i = 1; i <= N; i++) {
        sum += (double) 1 / i;
    }
    double end = omp_get_wtime();

    std::cout << "time " << end - start << std::endl;
    std::cout << "sum " << sum << std::endl;
}
