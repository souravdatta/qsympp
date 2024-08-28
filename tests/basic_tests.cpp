#include <iostream>

#include "utlite.hpp"
#include "../cmatrix.hpp"
#include "../core.hpp"

class CMatrixTests : public utlite::Suite<CMatrixTests>
{
public:
    void mat_creation()
    {
        qsym::cmatrix z{2, 2};
        z.set(0, 0, CMPLX(1, 0));
        UT_ASSERT(z.at(0, 0) == CMPLX(1, 0));
    }

    void mat_add()
    {
        qsym::cmatrix z{2, 2};
        z.set(0, 0, CMPLX(1, 0));
        qsym::cmatrix y{2, 2};
        y.set(0, 1, CMPLX(3, 4));
        auto r = z + y;
        auto expected = qsym::cmatrix(
            2,
            2,
            {CMPLX(1, 0),
             CMPLX(3, 4),
             CMPLX(0, 0),
             CMPLX(0, 0)});
        UT_ASSERT(r == expected);
    }

    void mat_minus()
    {
        qsym::cmatrix z{2, 2, {1, 2, 3, 4}};
        qsym::cmatrix y{2, 2, {4, 3, 2, 1}};
        auto r = z - y;
        auto expected = qsym::cmatrix(
            2,
            2,
            {CMPLXR(-3),
             CMPLXR(-1),
             CMPLXR(1),
             CMPLXR(3)});
        UT_ASSERT(r == expected);
    }

    void mat_mult()
    {
        qsym::cmatrix z{2, 2, {2, 3, 4, 5}};
        qsym::cmatrix y{2, 4, {1, 2, 3, 4, 4, 3, 2, 1}};
        auto r = z * y;
        auto expected = qsym::cmatrix(
            2,
            4,
            {14,
             13,
             12,
             11,
             24,
             23,
             22,
             21});
        UT_ASSERT(r == expected);
    }

    void mat_div()
    {
        qsym::cmatrix z{2, 4, {4, 8, 12, 16, 64, 16, 12, 8}};
        qsym::cmatrix y{2, 4, {4, 2, 4, 4, 4, 2, 2, 4}};
        auto r = qsym::element_wise(z, y, [=](std::complex<double> x, std::complex<double> y) -> std::complex<double>
                                    { return x / y; });
        auto expected = qsym::cmatrix(
            2,
            4,
            {1, 4, 3, 4, 16, 8, 6, 2});
        UT_ASSERT(r == expected);
    }

    void mat_tensor_product()
    {
        qsym::cmatrix m1{2, 2, {1, 2, 3, 4}};
        qsym::cmatrix m2{1, 3, {3, 2, 1}};
        auto r = qsym::tensor_product(m1, m2);
        UT_ASSERT(r.rows() == 2);
        UT_ASSERT(r.cols() == 6);
    }

    void qubit_q0_tensor_prod()
    {
        auto r = qsym::tensor_product(qsym::Qubit::Q0, qsym::Qubit::Q0);
        UT_ASSERT(r.rows() == 4);
        UT_ASSERT(r.cols() == 1);
        UT_ASSERT(r.at(0, 0) == CMPLX(1, 0));
        for (int i = 1; i < r.cols(); i++)
        {
            UT_ASSERT(r.at(i, 0) == CMPLXR(0));
        }
    }

    void qubit_q0_q1_tensor_prod()
    {
        auto r = qsym::tensor_product(qsym::Qubit::Q0, qsym::Qubit::Q1);
        UT_ASSERT(r.rows() == 4);
        UT_ASSERT(r.cols() == 1);
        UT_ASSERT(r.at(0, 1) == CMPLX(1, 0));
        for (int i = 0; i < r.cols(); i++)
        {
            if (i == 1)
                continue;
            UT_ASSERT(r.at(i, 0) == CMPLXR(0));
        }
    }

    void qubit_q1_q0_tensor_prod()
    {
        auto r = qsym::tensor_product(qsym::Qubit::Q1, qsym::Qubit::Q0);
        UT_ASSERT(r.rows() == 4);
        UT_ASSERT(r.cols() == 1);
        UT_ASSERT(r.at(0, 2) == CMPLX(1, 0));
        for (int i = 0; i < r.cols(); i++)
        {
            if (i == 2)
                continue;
            UT_ASSERT(r.at(i, 0) == CMPLXR(0));
        }
    }

    void qubit_q1_q1_tensor_prod()
    {
        auto r = qsym::tensor_product(qsym::Qubit::Q1, qsym::Qubit::Q1);
        UT_ASSERT(r.rows() == 4);
        UT_ASSERT(r.cols() == 1);
        UT_ASSERT(r.at(0, 3) == CMPLX(1, 0));
        for (int i = 0; i < r.cols(); i++)
        {
            if (i == 3)
                continue;
            UT_ASSERT(r.at(i, 0) == CMPLXR(0));
        }
    }

    SUITE_BEGIN
    UT(mat_creation, CMatrixTests);
    UT(mat_add, CMatrixTests);
    UT(mat_minus, CMatrixTests);
    UT(mat_mult, CMatrixTests);
    UT(mat_div, CMatrixTests);
    UT(mat_tensor_product, CMatrixTests);
    UT(qubit_q0_tensor_prod, CMatrixTests);
    UT(qubit_q0_q1_tensor_prod, CMatrixTests);
    UT(qubit_q1_q0_tensor_prod, CMatrixTests);
    UT(qubit_q1_q1_tensor_prod, CMatrixTests);
    SUITE_END
};

int main()
{
    CMatrixTests tests;
    tests.run();
}
