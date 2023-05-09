#include <iostream>
#include <exception>
#include "matrix.hpp"
#include "diff-scheme.h"

static inline double phi(double x) {
    return std::cos(M_PI * x);
}

static inline double psi(double t) {
    return std::exp(-t);
}

static inline double f(double t, double x) {
    return x + t;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        throw std::invalid_argument("Invalid number of arguments. Usage: seq arg1 arg2.");
    }
    const int tLen = static_cast<int>(std::strtol(argv[2], nullptr, 10));
    const int xLen = static_cast<int>(std::strtol(argv[1], nullptr, 10));
    const double T = 1.0;
    const double X = 1.0;
    const double alpha = 2.0;
    DiffScheme scheme(phi, psi, f, T, X, tLen, xLen, alpha);
    Matrix<double> matrix = scheme.SolveSystemRectScheme();
    matrix.Dump();
    return 0;
}
