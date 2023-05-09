#ifndef DIFFUSION_EQUATION_DIFF_SCHEME_H
#define DIFFUSION_EQUATION_DIFF_SCHEME_H

#include "matrix.hpp"

class DiffScheme {
    static class ConditionFunctions {
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
    const int tLen_;
    const int xLen_;
    const double alpha_;
    const double h_;
    const double tau_;
    const double beta_;

public:
    explicit DiffScheme(double (*phi)(double), double (*psi)(double), double (*f)(double, double),
                        const double T, const double X, const int tLen, const int xLen, const double alpha) :
                        condFuncs_(phi, psi, f), T_(T), X_(X), tLen_(tLen), xLen_(xLen), alpha_(alpha),
                        h_(X_ / (xLen_ - 1)), tau_(T_ / (tLen_ - 1)), beta_(tau_ / h_) {}

    void SolveSystemRectScheme(Matrix<double>& matrix) {
        InitMatrix(matrix);
        for (int t = 0; t < tLen_; ++t) {
            for (int x = 0; x < xLen_; ++x) {
                ComputePoint(matrix, t, x);
            }
        }
    }

    Matrix<double> SolveSystemRectScheme() {
        Matrix<double> matrix{tLen_, xLen_};
        InitMatrix(matrix);
        for (int t = 0; t < tLen_; ++t) {
            for (int x = 0; x < xLen_; ++x) {
                ComputePoint(matrix, t, x);
            }
        }
        return matrix;
    }

private:
    void InitMatrix(Matrix<double>& empty) const {
        for (int i = 0; i < tLen_; ++i) {
            empty[i][0] = condFuncs_.psi_(tau_ * i);
        }
        for (int i = 0; i < xLen_; ++i) {
            empty[0][i] = condFuncs_.phi_(h_ * i);
        }
    }

    void ComputePoint(Matrix<double> &matrix, int t, int x) const {
        if (x && t) {
            double eq_eq = matrix[t-1][x-1];
            double eq_plus = matrix[t-1][x];
            double plus_eq = matrix[t][x-1];
            matrix [t][x] = (2 * condFuncs_.f_(h_ * (x), tau_ * (t)) * tau_ - (plus_eq - eq_eq - eq_plus) -beta_ * alpha_ * (eq_plus - eq_eq - plus_eq)) / (1 + beta_ * alpha_);
        }
    }
};

#endif //DIFFUSION_EQUATION_DIFF_SCHEME_H
