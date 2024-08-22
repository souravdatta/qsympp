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

        Layer(size n) : _depth{n}
        {
            for (size i = 0; i < _depth; i++)
            {
                matrices.push_back(identity(2));
            }
        }

        void set_matrix(size index, const cmatrix &mat)
        {
            if ((index >= 0) && (index < _depth))
            {
                matrices[index] = mat;
            }
        }

        cmatrix get_matrix(size index)
        {
            if ((index >= 0) && (index < _depth))
            {
                return matrices[index];
            }

            return identity(2);
        }

        cmatrix operator_matrix() const
        {
            if (_depth <= 0)
                return identity(2);

            if (_depth == 1)
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
        size _depth;
    };

    class Circuit
    {
    public:
        Circuit() = delete;

        Circuit(size depth) : _depth{depth}, _last_layer{0}
        {
        }

        void set_matrix(size index, const cmatrix &mat)
        {
            if ((index < 0) || (index >= _depth))
                return;

            auto imat = identity(2);

            bool set = false;
            for (size i = 0; i < _layers.size(); i++)
            {
                Layer &layer = _layers[i];
                if (layer.get_matrix(index) == imat)
                {
                    if (mat != imat)
                    {
                        layer.set_matrix(index, mat);
                    }
                    set = true;
                    break;
                }
            }

            if (!set)
            {
                _layers.push_back(Layer{_depth});
                _layers[_layers.size() - 1].set_matrix(index, mat);
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

        void draw()
        {
            auto imat = identity(2);
            size layer_count = _layers.size();

            for (size i = 0; i < _depth; ++i)
            {
                for (size j = 0; j < layer_count; ++j)
                {
                    auto mat = _layers[j].get_matrix(i);
                    if (mat == Matrix::H)
                    {
                        std::cout << "H\t";
                    }
                    else if (mat == Matrix::X)
                    {
                        std::cout << "X\t";
                    }
                    else if (mat == imat)
                    {
                        std::cout << "I\t";
                    }
                    else
                    {
                        std::cout << "Matrix[" << mat.rows() << "x" << mat.cols() << "]\t";
                    }
                }

                std::cout << "\n";
            }
        }

    private:
        size _depth;
        size _last_layer;
        std::vector<Layer> _layers;
    };

    class SVSimulator;
}

#endif
