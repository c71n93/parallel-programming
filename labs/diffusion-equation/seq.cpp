#include <iostream>
#include <exception>
#include "matrix.hpp"
#include "diff-equation.hpp"

#if 0
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
#endif

double phi(double x) {
    return exp(-x);
}
double psi(double t) {
    return exp(t);
}
double f(double  x, double t) {
    return x * t;
}

inline std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> meshgrid(const std::vector<double>& tVec,
                                                                                       const std::vector<double>& xVec) {
    std::vector<std::vector<double>> tGrid;
    std::vector<std::vector<double>> xGrid;
    for (const auto& t : tVec) {
        std::vector<double> row;
        for (const auto& x : xVec) {
            row.push_back(x);
        }
        xGrid.push_back(row);
    }
    for (const auto& x : xVec) {
        std::vector<double> row;
        for (const auto& t : tVec) {
            row.push_back(t);
        }
        tGrid.push_back(row);
    }
    return std::make_pair(tGrid, xGrid);
}
inline std::vector<double> makeVecFromInterval(double interval, int amount) {
    std::vector<double> v(amount);
    for (int i = 0; i < amount; i++) {
        v.push_back(interval * i);
    }
    return v;
}

void makePlot(const Matrix<double>& matrix, double T, double X, int tLen, int xLen) {
    double tau = T / (tLen - 1);
    double h = X / (xLen - 1);
    std::vector<double> tVec = makeVecFromInterval(tau, tLen);
    std::vector<double> xVec = makeVecFromInterval(h, xLen);
    auto gridPair = meshgrid(tVec, xVec);
    auto matrixVec = matrix.ToVector();
#if 0
    plt::plot_surface(gridPair.first, gridPair.second, matrixVec);
    plt::show();
#endif
}

int main(int argc, char **argv) {
    if (argc != 3) {
        throw std::invalid_argument("Invalid number of arguments. Usage: seq arg1 arg2.");
    }
    const int tLen = static_cast<int>(std::strtol(argv[2], nullptr, 10));
    const int xLen = static_cast<int>(std::strtol(argv[1], nullptr, 10));
    const double T = 1.0;
    const double X = 1.0;
    const double alpha = 0.5;
    DiffEquationSystem equationSystem(phi, psi, f, T, X, alpha);
    Matrix<double> matrix = equationSystem.SolveSystemRectScheme(tLen, xLen);
#if 0
    makePlot(matrix, T, X, tLen, xLen);
#endif
    return 0;
}
