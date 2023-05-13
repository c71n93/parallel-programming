#include <iostream>
#include <exception>
#include <thread>
#include <future>

double calculatePartialSum(size_t N, size_t tAmount, size_t tNum) {
    double partialSum = 0.0;
    for (size_t i = tNum; i <= N; i += tAmount) {
        partialSum += 1.0 / static_cast<double>(i);
    }
    return partialSum;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::invalid_argument("Invalid number of arguments. Number of threads is required.");
    }
    const size_t nTreads = static_cast<size_t>(std::strtol(argv[1], nullptr, 10));
    const size_t N = static_cast<size_t>(std::strtol(argv[2], nullptr, 10));

    double sum = 0.0;
    std::vector<std::future<double>> futVec(nTreads);
    for (size_t i = 0; i < nTreads; i++) {
        futVec[i] = std::async(calculatePartialSum, N, nTreads, i + 1);
    }
    for (auto& f : futVec) {
        sum += f.get();
    }
    std::cout << "sum = " << sum << std::endl;
    return 0;
}