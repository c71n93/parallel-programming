#include <iostream>
#include <exception>
#include <thread>
#include <mutex>

std::mutex m;
void threadHello(size_t tAmount, size_t tNum) {
    std::lock_guard<std::mutex> guard(m);
    std::cout << "Hello, world! " << "Treads amount: " << tAmount << " My num: " << tNum << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::invalid_argument("Invalid number of arguments. Number of threads is required.");
    }
    const size_t nTreads = static_cast<size_t>(std::strtol(argv[1], nullptr, 10));

    std::vector<std::thread> thVec(nTreads);
    for (size_t i = 0; i < nTreads; i++) {
        thVec[i] = std::thread(threadHello, nTreads, i);
    }
    for (auto& t: thVec) {
        t.join();
    }

    return 0;
}
