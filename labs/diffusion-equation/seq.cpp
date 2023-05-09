#include <iostream>
#include <exception>
#include "matrix.hpp"
#include "diff-equation.h"

double phi(double x) {
    return std::cos(M_PI * x);
}

double psi(double t) {
    return std::exp(-t);
}

double f(double t, double x) {
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
    DiffEquationSystem equationSystem(phi, psi, f, T, X, alpha);
    Matrix<double> matrix = equationSystem.SolveSystemRectScheme(tLen, xLen);
    matrix.Dump();
    return 0;
}
