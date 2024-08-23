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
                _matrices.push_back(identity(2));
            }
        }

        void set_matrix(size index, const cmatrix &mat)
        {
            if ((index >= 0) && (index < _depth))
            {
                _matrices[index] = mat;
            }
        }

        cmatrix get_matrix(size index)
        {
            if ((index >= 0) && (index < _depth))
            {
                return _matrices[index];
            }

            return identity(2);
        }

        cmatrix operator_matrix() const
        {
            if (_depth <= 0)
                return identity(2);

            if (_depth == 1)
                return _matrices[0];

            std::vector<cmatrix> rev_layer;
            for (cmatrix c : _matrices)
            {
                rev_layer.push_back(c);
            }
            std::reverse(rev_layer.begin(), rev_layer.end());

            auto start = rev_layer[0];
            return std::accumulate(rev_layer.begin() + 1, rev_layer.end(), start, [=](auto m1, auto m2)
                                   { return tensor_product(m1, m2); });
        }

        cmatrix state_vector(const cmatrix &input) const
        {
            auto gate = operator_matrix();
            return gate * input;
        }

    private:
        std::vector<cmatrix> _matrices;
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
                    std::cout << mat.name() << "\t";
                }

                std::cout << "\n";
            }
        }

        cmatrix state_vector(const cmatrix &mat) const
        {
            return std::accumulate(
                _layers.begin(),
                _layers.end(),
                mat,
                [=](const cmatrix &m, const Layer &layer) -> cmatrix
                {
                    return layer.state_vector(m);
                });
        }

    private:
        size _depth;
        size _last_layer;
        std::vector<Layer> _layers;
    };
}

#endif
