#include "cmatrix.hpp"
#include "core.hpp"
#include "circuit.hpp"
#include <iostream>

int main()
{
    qsym::cmatrix z{2, 2};
    z.set(0, 0, CMPLX(1, 0));
    qsym::cmatrix y{2, 2};
    y.set(0, 1, CMPLX(3, 4));
    std::cout << (z + y) << "\n";

    qsym::cmatrix m1{2, 2};
    qsym::cmatrix m2{2, 3};
    m1.set(0, 0, 1).set(0, 1, 2).set(1, 0, 3).set(1, 1, 4);
    std::cout << m1 << "\n";
    m2.set(0, 0, 1).set(0, 1, 2).set(0, 2, 3);
    m2.set(1, 0, 4).set(1, 1, 5).set(1, 2, 6);
    std::cout << m2 << "\n";
    auto r = qsym::tensor_product(m1, m2);
    std::cout << r << "\n";

    std::cout << (qsym::Matrix::X * qsym::Qubit::Q0) << "\n";

    std::cout << qsym::identity(4) << "\n";

    auto sv = qsym::Matrix::H * qsym::Qubit::Q0;
    std::cout << sv << "\n";
    std::cout << qsym::measure_state(sv).value_or("???") << "\n";

    auto counts = qsym::counts(sv);
    for (auto m : counts)
    {
        std::cout << m.first << " => " << m.second << "\n";
    }

    auto gate = qsym::tensor_product(qsym::tensor_product(qsym::Matrix::H, qsym::Matrix::H),
                                     qsym::Matrix::X);
    auto input = qsym::tensor_product(qsym::tensor_product(qsym::Qubit::Q0, qsym::Qubit::Q0),
                                      qsym::Qubit::Q0);
    auto sv2 = gate * input;
    std::cout << sv2 << "\n";
    std::cout << qsym::measure_state(sv2).value_or("???") << "\n";

    auto counts2 = qsym::counts(sv2);
    for (auto m : counts2)
    {
        std::cout << m.first << " => " << m.second << "\n";
    }

    std::cout << "==============\n";

    qsym::Layer layer{3};
    layer.x(0);
    layer.h(2);
    layer.h(1);
    auto q0 = qsym::Qubit::Q0;
    auto sv3 = layer.state_vector(qsym::ground_qubits(3));
    auto counts3 = qsym::counts(sv3);
    for (auto m : counts3)
    {
        std::cout << m.first << " => " << m.second << "\n";
    }
    // // TODO: noting down the API
    // qsym::Circuit<3> circuit;
    // circuit.h(0);
    // circuit.h(1);
    // circuit.x(2);

    // qsym::SVSimulator sim;
    // auto sv = sim.run(circuit);
    // auto counts = qsym::counts(sv);
}
