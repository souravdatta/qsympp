#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "core.hpp"
#include "cmatrix.hpp"
#include <vector>
#include <algorithm>

namespace qsym
{
    class Layer
    {
    public:
        Layer() = delete;

        Layer(size n) : _circuit_depth{n}
        {
            for (size i = 0; i < _circuit_depth; i++)
            {
                matrices.push_back(identity(2));
            }
        }

        void set_matrix(size index, const cmatrix &mat)
        {
            if (index >= 0 && index < _circuit_depth)
            {
                matrices[index] = mat;
            }
        }

        void x(size index)
        {
            set_matrix(index, Matrix::X);
        }

        void h(size index)
        {
            set_matrix(index, Matrix::H);
        }

        cmatrix get_matrix(size index)
        {
            if (index >= 0 && index < _circuit_depth)
            {
                return matrices[index];
            }

            return identity(2);
        }

        cmatrix operator_matrix() const
        {
            if (_circuit_depth <= 0)
                return identity(2);

            if (_circuit_depth == 1)
                return matrices[0];

            auto start = matrices[0];
            return std::accumulate(matrices.begin() + 1, matrices.end(), start, [=](auto m1, auto m2)
                                   { return tensor_product(m1, m2); });
        }

        cmatrix state_vector(const cmatrix &input) const
        {
            auto gate = operator_matrix();
            return gate * input;
        }

    private:
        std::vector<cmatrix> matrices;
        size _circuit_depth;
    };

    class Circuit;

    class SVSimulator;
}

#endif