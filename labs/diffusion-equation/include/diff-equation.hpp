#ifndef DIFFUSION_EQUATION_DIFF_EQUATION_HPP
#define DIFFUSION_EQUATION_DIFF_EQUATION_HPP

#include "matrix.hpp"

class DiffEquationSystem {
    class ConditionFunctions {
    public:
        double (*phi_)(double);
        double (*psi_)(double);
        double (*f_)(double, double);

        ConditionFunctions(double (*phi)(double), double (*psi)(double), double (*f)(double, double)) : phi_(phi),
        psi_(psi), f_(f) {}
    };

    ConditionFunctions condFuncs_;
    const double T_;
    const double X_;
    const double alpha_;

public:
    explicit DiffEquationSystem(double (*phi)(double), double (*psi)(double), double (*f)(double, double),
                                const double T, const double X, const double alpha) :
                        condFuncs_(phi, psi, f), T_(T), X_(X), alpha_(alpha) {}

    void SolveSystemRectScheme(Matrix<double>& matrix, int tLen, int xLen) {
        double h = X_ / (xLen - 1);
        double tau = T_ / (tLen - 1);
        double beta = tau / h;
        InitConditions(matrix, tLen, xLen, tau, h);
        for (int t = 0; t < tLen; ++t) {
            for (int x = 0; x < xLen; ++x) {
                ComputePoint(matrix, t, x, tau, h, beta);
            }
        }
    }

    Matrix<double> SolveSystemRectScheme(int tLen, int xLen) {
        Matrix<double> matrix{tLen, xLen};
        double h = X_ / (xLen - 1);
        double tau = T_ / (tLen - 1);
        double beta = tau / h;
        InitConditions(matrix, tLen, xLen, tau, h);
        for (int t = 0; t < tLen; ++t) {
            for (int x = 0; x < xLen; ++x) {
                ComputePoint(matrix, t, x, tau, h, beta);
            }
        }
        return matrix;
    }

private:
    void InitConditions(Matrix<double>& matrix, int tLen, int xLen, double tau, double h) const {
        for (int i = 0; i < tLen; ++i) {
            matrix[i][0] = condFuncs_.psi_(tau * i);
        }
        for (int i = 0; i < xLen; ++i) {
            matrix[0][i] = condFuncs_.phi_(h * i);
        }
    }

    void ComputePoint(Matrix<double> &matrix, int t, int x, double tau, double h, double beta) const {
        if (x && t) {
            double eq_eq = matrix[t-1][x-1];
            double eq_plus = matrix[t-1][x];
            double plus_eq = matrix[t][x-1];
            matrix [t][x] = (2 * condFuncs_.f_(h * (x), tau * (t)) * tau - (plus_eq - eq_eq - eq_plus) - beta * alpha_ * (eq_plus - eq_eq - plus_eq)) / (1 + beta * alpha_);
        }
    }
};

#endif //DIFFUSION_EQUATION_DIFF_EQUATION_HPP
