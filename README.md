## qsympp - a tiny quantum simulator in C++ 17

The library is header only and inlines most of the methods.

#### Tests

The usage of the core modules can be understood via the unit tests. How to run the tests:
1. Goto tests. `cd tests`.
2. Set CPP environment variable to your C++ implementation. We need it to be C++17 compliant.
3. Run the tests via Makefile target. `CPP=g++ make tests` or `CPP=clang++ make tests`.

#### Example

The simple example:
```c++
qsym::Circuit c2{2};
c2.x(0);
c2.h(1);
c2.h(0);
c2.x(1);
auto opt_sv = c2.state_vector(qsym::ground_qubits(2));
auto opt_counts = qsym::counts(opt_sv);
```
