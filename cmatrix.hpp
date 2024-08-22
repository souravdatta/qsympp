#ifndef __CMATRIX_HPP__
#define __CMATRIX_HPP__

#include <string>
#include <complex>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <initializer_list>

#define CMPLX(X, Y) \
    std::complex<double> { (X), (Y) }

namespace qsym
{
    using size = unsigned long;

    class cmatrix
    {
    public:
        cmatrix() = delete;

        cmatrix(size rn, size cn) : _rows{rn}, _cols{cn}
        {
            reset();
        }

        cmatrix(size rn, size cn, std::vector<std::complex<double>> nums) : _rows{rn}, _cols{cn}
        {
            if (nums.size() != _rows * _cols)
                return;

            auto elem_ptr = nums.begin();
            reset();

            for (size i = 0; i < _rows; i++)
            {
                for (size j = 0; j < _cols; j++)
                {
                    this->set(i, j, *elem_ptr++);
                }
            }
        }

        void reset()
        {
            _mat.resize(_rows);
            for (size i = 0; i < _rows; i++)
            {
                _mat[i].resize(_cols);
                for (size j = 0; j < _cols; j++)
                {
                    _mat[i][j] = CMPLX(0, 0);
                }
            }
        }

        std::complex<double> at(size r, size c) const
        {
            return _mat[r][c];
        }

        cmatrix &set(size r, size c, const std::complex<double> &v)
        {
            _mat[r][c] = v;
            return *this;
        }

        size rows() const
        {
            return _rows;
        }

        size cols() const
        {
            return _cols;
        }

        bool operator==(const cmatrix &other) const
        {
            if ((_rows == other.rows()) &&
                (_cols == other.cols()))
            {
                return _mat == other._mat;
            }

            return false;
        }

        bool operator!=(const cmatrix &other) const
        {
            return !(*this == other);
        }

    private:
        size _rows, _cols;
        std::vector<std::vector<std::complex<double>>> _mat;
    };

    cmatrix identity(size n)
    {
        cmatrix mat{n, n};
        for (size i = 0; i < n; i++)
        {
            mat.set(i, i, CMPLX(1, 0));
        }
        return mat;
    }

    std::ostream &operator<<(std::ostream &os, const cmatrix &c)
    {
        os << "@matrix [" << c.rows() << ", " << c.cols() << "]--\n";
        for (size i = 0; i < c.rows(); i++)
        {
            for (size j = 0; j < c.cols(); j++)
            {
                os << c.at(i, j) << " ";
            }
            os << "\n";
        }
        os << "--@matrix\n";
        return os;
    }

    cmatrix element_wise(
        const cmatrix &m1,
        const cmatrix &m2,
        std::function<std::complex<double>(std::complex<double>, std::complex<double>)> f)
    {
        if (m1.rows() != m2.rows() ||
            m1.cols() != m2.cols())
        {
            throw std::string{"Unmatched dimensions"};
        }
        cmatrix result{m1.rows(), m1.cols()};

        for (size i = 0; i < result.rows(); i++)
        {
            for (size j = 0; j < result.cols(); j++)
            {
                result.set(i, j, f(m1.at(i, j), m2.at(i, j)));
            }
        }

        return result;
    }

    cmatrix operator+(const cmatrix &m1,
                      const cmatrix &m2)
    {
        return element_wise(m1, m2, [=](auto c1, auto c2)
                            { return c1 + c2; });
    }

    cmatrix operator-(const cmatrix &m1,
                      const cmatrix &m2)
    {
        return element_wise(m1, m2, [=](auto c1, auto c2)
                            { return c1 - c2; });
    }

    cmatrix tensor_product(const cmatrix &m1, const cmatrix &m2)
    {
        cmatrix result{m1.rows() * m2.rows(), m1.cols() * m2.cols()};

        for (size i = 0; i < m1.rows(); i++)
        {
            for (size j = 0; j < m1.cols(); j++)
            {
                auto p1 = m1.at(i, j);
                for (size k = 0; k < m2.rows(); k++)
                {
                    for (size l = 0; l < m2.cols(); l++)
                    {
                        auto p2 = m2.at(k, l);
                        auto r = p1 * p2;
                        result.set(i * m2.rows() + k, j * m2.cols() + l, r);
                    }
                }
            }
        }

        return result;
    }

    cmatrix operator*(const cmatrix &m1,
                      const cmatrix &m2)
    {
        if (m1.cols() != m2.rows())
            throw std::string{"col count != row count"};

        cmatrix result{m1.rows(), m2.cols()};
        for (size i = 0; i < m1.rows(); i++)
        {
            for (size j = 0; j < m2.cols(); j++)
            {
                std::complex<double> sum{0, 0};
                for (size k = 0; k < m1.cols(); k++)
                {
                    sum += m1.at(i, k) * m2.at(k, j);
                }
                result.set(i, j, sum);
            }
        }

        return result;
    }
}

#endif
