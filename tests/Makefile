tests : basic_tests circuit_tests
	./basic_tests
	./circuit_tests

basic_tests : basic_tests.cpp
	$(CPP) -std=c++17 -o basic_tests basic_tests.cpp

circuit_tests : circuit_tests.cpp
	$(CPP) -std=c++17 -o circuit_tests circuit_tests.cpp

.PHONY : clean clobber
clean :
	rm -fr basic_tests circuit_tests

