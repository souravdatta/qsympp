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

    SUITE_BEGIN
    UT(mat_creation, CMatrixTests);
    UT(mat_add, CMatrixTests);
    SUITE_END
};

int main()
{
    CMatrixTests tests;
    tests.run();
}
