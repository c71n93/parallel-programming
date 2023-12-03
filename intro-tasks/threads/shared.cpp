#include <iostream>
#include <exception>
#include <thread>
#include <mutex>

std::mutex m;
void increment(int& sharedData, size_t tNum) {
    std::lock_guard<std::mutex> guard(m);
    sharedData++;
    std::cout << "My num: " << tNum << " value = " << sharedData << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::invalid_argument("Invalid number of arguments. Number of threads is required.");
    }
    const size_t nTreads = static_cast<size_t>(std::strtol(argv[1], nullptr, 10));

    int sharedData = 0;

    std::vector<std::thread> thVec(nTreads);
    for (size_t i = 0; i < nTreads; i++) {
        thVec[i] = std::thread(increment, std::ref(sharedData), i + 1);
    }
    for (auto& t: thVec) {
        t.join();
    }

    return 0;
}