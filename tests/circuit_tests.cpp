#include "utlite.hpp"
#include "../cmatrix.hpp"
#include "../core.hpp"
#include "../circuit.hpp"

class CircuitTests : public utlite::Suite<CircuitTests>
{
public:
    void simple_gated_circuit()
    {
        qsym::Circuit c2{2};
        c2.x(0);
        c2.h(1);
        c2.h(0);
        c2.x(1);
        auto opt_sv = c2.state_vector(qsym::ground_qubits(2));
        auto opt_counts = qsym::counts(opt_sv);
        UT_ASSERT(opt_counts.size() == 4);
    }

    SUITE_BEGIN
    UT(simple_gated_circuit, CircuitTests);
    SUITE_END
};

int main()
{
    CircuitTests tests;
    tests.run();
}
