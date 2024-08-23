#ifndef __CORE_HPP__
#define __CORE_HPP__

#include "cmatrix.hpp"
#include <string>
#include <vector>
#include <tuple>
#include <random>
#include <optional>
#include <map>
#include <cmath>

namespace qsym
{
    class Matrix
    {
    public:
        const static cmatrix X;
        const static cmatrix Y;
        const static cmatrix Z;
        const static cmatrix H;
        const static cmatrix S;
        const static cmatrix T;
    };

    const cmatrix Matrix::X{"X", 2, 2, {CMPLX(0, 0), CMPLX(1, 0), CMPLX(1, 0), CMPLX(0, 0)}};
    const cmatrix Matrix::Y{"Y", 2, 2, {CMPLX(0, 0), CMPLX(0, -1), CMPLX(0, 1), CMPLX(0, 0)}};
    const cmatrix Matrix::Z{"Z", 2, 2, {CMPLX(1, 0), CMPLX(0, 0), CMPLX(0, 0), CMPLX(-1, 0)}};

    const double h_factor = 1.0 / std::sqrt(2);

    const cmatrix Matrix::H{
        "H",
        2,
        2,
        {CMPLX(h_factor, 0), CMPLX(h_factor, 0), CMPLX(h_factor, 0), CMPLX(-1 * h_factor, 0)}};

    class Qubit
    {
    public:
        const static cmatrix Q0;
        const static cmatrix Q1;
    };

    const cmatrix Qubit::Q0{2, 1, {CMPLX(1, 0), CMPLX(0, 0)}};
    const cmatrix Qubit::Q1{2, 1, {CMPLX(0, 0), CMPLX(1, 0)}};

    namespace utils
    {
        std::string make_binary(size len, size n)
        {
            std::string s;
            do
            {
                s = std::to_string(n & 1) + s;
            } while (n >>= 1);

            if (s.size() < len)
            {
                auto diff = len - s.size();
                for (unsigned long i = 0; i < diff; ++i)
                {
                    s = "0" + s;
                }
            }
            return s;
        }

        std::vector<std::string> bit_strings(size n)
        {
            std::vector<std::string> arr;
            size nbits = static_cast<size>(std::pow(2, n));

            for (size i = 0; i < nbits; ++i)
            {
                arr.push_back(make_binary(n, i));
            }

            return arr;
        }

        size roll()
        {
            std::random_device dev;
            std::mt19937 rng{dev()};
            std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 100);
            return static_cast<size>(dist6(rng));
        }
    }

    std::optional<std::string> measure_state(const cmatrix &m)
    {
        if (m.cols() > 1)
            return {};

        size nbits = static_cast<size>(std::log2(m.rows()));
        auto bit_values = utils::bit_strings(nbits);
        double dice = utils::roll();
        std::vector<std::tuple<double, std::string>> measures;
        for (size i = 0; i < m.rows(); ++i)
        {
            if (m.at(i, 0) == CMPLX(0, 0))
                continue;
            auto v = std::abs(m.at(i, 0));
            measures.push_back(std::tuple<double, std::string>{v * v * 100, bit_values[i]}); // scale by 100
        }

        // std::cout << "DICE: " << dice << "\n";

        double start = 0;
        for (auto p : measures)
        {
            // std::cout << " (" << start << ", " << start + std::get<0>(p) << ")\n";
            if ((dice > start) && (dice <= start + std::get<0>(p)))
            {
                // std::cout << std::get<1>(p) << "\n";
                return std::get<1>(p);
            }
            start = start + std::get<0>(p);
        }

        return {};
    }

    std::map<std::string, size> counts(const cmatrix &m, unsigned long shots = 1024)
    {
        std::map<std::string, size> count_map;

        for (unsigned long shot = 0l; shot < shots; ++shot)
        {
            std::optional<std::string> result = measure_state(m);
            if (result.has_value())
            {
                auto measurement = result.value();
                count_map[measurement] += 1;
            }
        }

        return count_map;
    }

    cmatrix ground_qubits(size n)
    {
        auto qbit = Qubit::Q0;
        size i = 0;

        while (i++ < n - 1)
        {
            qbit = tensor_product(qbit, Qubit::Q0);
        }

        return qbit;
    }

}

#endif
