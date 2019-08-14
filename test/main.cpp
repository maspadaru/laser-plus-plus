#include <iostream>

#include <acyclicity/naive_smfa.h>
#include <core/reasoner.h>
#include <example/example_io_manager.h>
#include <rule/rule_parser.h>
#include <util/chase_algorithm.h>

void run(std::string const &name, std::string const &stream_string,
         std::string const &rule_string,
         laser::util::ChaseAlgorithm chase_algorithm) {
    std::cout << std::endl;
    std::cout << " Test: " << name << std::endl;
    std::cout << " =================================== " << std::endl;
    // It might not be very intuitive, but chase_algorithm needs to be set
    // before parsing the rules !!!
    // As a general rule set the settings before initializing anything else.
    // I should make this more obvious somehow...
    laser::util::Settings::get_instance().set_chase_algorithm(chase_algorithm);
    auto example_io_manager = laser::example::ExampleIOManager(stream_string);
    auto rule_parser = laser::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto reasoner = laser::core::Reasoner(&rule_vector, &example_io_manager);
    reasoner.start();
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void run_acyclicity_test(std::string const &name,
                         std::string const &rule_string, bool expected) {
    bool acyclicity_result;
    std::cout << std::endl;
    std::cout << " Acyclicity Test: " << name << std::endl;
    laser::util::Settings::get_instance().set_chase_algorithm(
        laser::util::ChaseAlgorithm::SKOLEM);
    auto rule_parser = laser::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto naive_smfa = laser::acyclicity::NaiveSMFA(rule_vector);
    acyclicity_result = naive_smfa.has_terminating_chase();
    std::cout << " Naive_SMFA returned: " << acyclicity_result
              << " expected: " << expected << ";" << std::endl;
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl;
}

void test_acyclicity_simple() {
    const std::string name = "Acyclicity Simple";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    bool expected_has_terminating_chase = true;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_obvious_cycle() {
    const std::string name = "Acyclicity Obvious Cycle";
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_obvious_cycle_with_time() {
    const std::string name = "Acyclicity Obvious Cycle With Time";
    std::string rule_string = "p(X) := [$, 3][B]r(X)\n"
                              "q(Y) && m(X, Y) := [$, 5][D]p(X)\n"
                              "p(Y) && n(X, Y) := [$, 5][D]q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_only_first_timepoint() {
    // In this test, a cycle will only occur at the first timepoint
    const std::string name = "Acyclicity Only First Timepoint";
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := [$, 3][B]q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_simple_mix() {
    const std::string name = "Simple Mix";
    std::string stream_string = "1 4 "
                                "1 : a(x1, y1, z1)\n"
                                "2 : a(x2, y2, z2)\n"
                                "3 : a(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(Z, X, Y) := a(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_simple() {
    const std::string name = "Simple";
    std::string stream_string = "1 4 "
                                "1 : a(x1, y1, z1)\n"
                                "2 : a(x2, y2, z2)\n"
                                "3 : a(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(X, Y, Z) := a(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_queries() {
    const std::string name = "Queries";
    std::string stream_string = "1 6 "
                                "1 : a(x1, 1, z1, 1)\n"
                                "2 : a(x2, y2, z2, 1)\n"
                                "3 : a(x3, 3, z3, 1), a(x3, 1, z3, 1)\n"
                                "4 : a(x41, y41, z41, y41), a(x42, 1, z42, 1)\n"
                                "4 : a(x43, 1, z43, 2), a(x44, y44, z44, 2)\n"
                                "5 : a(x5, y5, z5, 1), p(a, 1, b, 1)\n";
    std::string rule_string = "p(A, B, C, D) := a(A, B, C, D)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_atoms() {
    const std::string name = "Atoms";
    std::string stream_string = "1 14 "
                                "1 : f(x1)\n"
                                "2 : c(x2, y2)\n"
                                "2 : d(x2, y2)\n"
                                "4 : \n"
                                "5 : \n"
                                "5 : e(x5, y5), e(a5, b5), e(x5, y5)\n"
                                "5 : e(x5, y5)\n"
                                "6 : a(x6, y6, z6)\n"
                                "7 : \n"
                                "8 : a(x8, y8, z8), b(y8, z8)\n"
                                "9 : \n"
                                "10 : a(x10, y10, z10)\n"
                                "11 : f(a11)\n"
                                "12 : e(x12, x12)\n"
                                "13 : \n"
                                "14 : \n";
    std::string rule_string = "q(X, Y, Z) := a(X, Y, Z)\n"
                              "r(Y) := c(X, Y)\n"
                              "s(X) := e(X, X)\n"
                              "t(Y, X) := d(X, Y)\n"
                              "u(X, X) := f(X)\n"
                              "v(X, Y, X, Y) := e(X, Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_diamond_atom() {
    const std::string name = "Diamond Atom";
    std::string stream_string = "1 14 "
                                "1 : \n"
                                "2 : c(x2, y2)\n"
                                "2 : d(x2, y2)\n"
                                "4 : \n"
                                "5 : \n"
                                "5 : e(x5, y5), e(a5, b5), e(x5, y5)\n"
                                "5 : e(x5, y5)\n"
                                "6 : a(x6, y6, z6)\n"
                                "7 : \n"
                                "8 : a(x8, y8, z8), b(y8, z8)\n"
                                "9 : \n"
                                "10 : a(x10, y10, z10)\n"
                                "11 : f(a11)\n"
                                "11 : f(-9.099)\n"
                                "11 : f(Z)\n"
                                "12 : e(x12, x12)\n"
                                "13 : \n"
                                "14 : \n";
    std::string rule_string = "q(X, Y, Z) := [D] a(X, Y, Z)\n"
                              "u(X, X) := [D]f(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_time_window() {
    const std::string name = "Time Window";
    std::string stream_string = "1 14 "
                                "1 : \n"
                                "2 : c(x2, y2)\n"
                                "2 : d(x2, y2)\n"
                                "4 : \n"
                                "5 : \n"
                                "5 : e(x5, y5), e(a5, b5), e(x5, y5)\n"
                                "5 : e(x5, y5)\n"
                                "6 : a(x6, y6, z6)\n"
                                "7 : \n"
                                "8 : a(x8, y8, z8), b(y8, z8)\n"
                                "9 : \n"
                                "10 : a(x10, y10, z10)\n"
                                "11 : f(a11)\n"
                                "11 : f(-9.099)\n"
                                "11 : f(Z)\n"
                                "12 : e(x12, x12)\n"
                                "13 : \n"
                                "14 : \n";
    std::string rule_string = "q(X, Y, Z) := [$, 3] [D] a(X, Y, Z)\n"
                              "r(Y) := [$, 100] c(X, Y)\n"
                              "s(X) := [$, 0] [D] e(X, X)\n"
                              "u(X, X) := [$, 1][D]f(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_box() {
    const std::string name = "Box";
    std::string stream_string = "1 14 "
                                "1 : f(1), f(a), c(a, b)\n"
                                "2 : f(a), f(1), c(a, b), f(x2)\n"
                                "3 : f(a), f(1), c(a, b), f(x2)\n"
                                "4 : f(a), f(1), c(b, a), f(x2)\n"
                                "5 : f(1), f(a), c(a,b), c(b, a)\n"
                                "6 : f(1), c(b,a), f(a)\n"
                                "7 : f(a)\n"
                                "8 : f(1), f(a), c(a, b), c(6,7), c(1,2)\n"
                                "9 : f(1), f(a), c(a, b), c(6,7), c(1,2)\n"
                                "10 : f(1), f(a), c(a, b), c(6,7), c(1,2)\n"
                                "11 : f(1), f(a), c(6,7), c(1,2)\n"
                                "12 : f(1), f(a), c(6,7)\n"
                                "13 : f(1), f(a)\n"
                                "14 : f(1), f(a)\n";
    std::string rule_string = ""
                              "q(X) := [B] f(X)\n"
                              "r(Y,X) := [$, 3][B]c(X, Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_same_variables() {
    const std::string name = "Simple";
    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(x)\n"
                                "3 : a(x)\n"
                                "4 : a(x)\n"
                                "5 : a(x), b(x)\n"
                                "6 : b(x)\n"
                                "7 : b(x)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(x)\n"
                                "11 : a(x)\n"
                                "12 : a(x)\n"
                                "13 : a(x)\n"
                                "14 : a(x)\n";
    std::string rule_string = "q(X) := a(X) &&  b(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_two_variables() {
    const std::string name = "Conjunction two variables";
    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(y)\n"
                                "3 : a(x)\n"
                                "4 : a(x)\n"
                                "5 : a(x), b(y)\n"
                                "6 : b(x)\n"
                                "7 : b(x)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(y)\n"
                                "11 : a(x)\n"
                                "12 : a(x)\n"
                                "13 : a(x)\n"
                                "14 : a(x)\n";
    std::string rule_string = "q(X,Y) := a(X) &&  b(Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_diamond() {
    const std::string name = "Conjunction Diamond";
    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(y)\n"
                                "3 : a(x)\n"
                                "4 : a(x)\n"
                                "5 : a(x), b(y)\n"
                                "6 : b(y)\n"
                                "7 : b(y)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(y)\n"
                                "11 : a(x)\n"
                                "12 : a(x)\n"
                                "13 : a(x)\n"
                                "14 : a(x)\n";
    std::string rule_string = "q(X,Y) := a(X) && [$, 2] [D] b(Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_box() {
    const std::string name = "Conjunction Box";
    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(y)\n"
                                "3 : a(x), b(y)\n"
                                "4 : a(x), b(y)\n"
                                "5 : a(x), b(y)\n"
                                "6 : b(y)\n"
                                "7 : b(y)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(y)\n"
                                "11 : a(x), b(y)\n"
                                "12 : a(x), b(y)\n"
                                "13 : a(x), b(y)\n"
                                "14 : a(x)\n";
    std::string rule_string = "q(X,Y) := a(X) && [$, 2] [B] b(Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_corss_variables() {
    const std::string name = "Conjunction cross variables";
    std::string stream_string = "1 14 "
                                "1 : a(x,y)\n"
                                "2 : a(x,x), b(y,x)\n"
                                "3 : a(y,y), b(y,x)\n"
                                "4 : a(x,y), b(y,x)\n"
                                "5 : a(y,x), b(y,x)\n"
                                "6 : a(x,x), b(x,y)\n"
                                "7 : a(x,x), b(y,y)\n"
                                "8 : a(x,x), b(y,x)\n"
                                "9 : b(x,y)\n"
                                "10 : \n";
    std::string rule_string = "q(X,Y) := a(X,X) && b(Y,X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_sne() {
    const std::string name = "Simple";
    std::string stream_string = "1 4 "
                                "1 : p(x1)\n"
                                "2 : t(x1)\n"
                                "3 : s(x1)\n"
                                "4 : \n";
    std::string rule_string =
        "q(X) := t(X) && [$, 3] [D] p(X)\n"  // old input fact
        "r(X) := s(X) && [$, 2] [D] q(X)\n"; // old conclusion
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_transitive() {
    const std::string name = "Conjunction Transitive";
    std::string stream_string =
        "1 4 "
        "1 : p(a1, a2), p(a2, a3)\n"
        "2 : p(b1,b2), p(b2, b3), p(b3, b4)\n"
        "3 : p(c1,c2), p(c2, c3), p(c3, c4), p(c4, c5), p(c5, c6)\n"
        "4 : \n";
    std::string rule_string = "p(X, Z) := p(X, Y) && p(Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_recursive_simple() {
    const std::string name = "Recursive Simple";
    std::string stream_string = "1 6 "
                                "1 : p(1), q(1)\n"
                                "2 : p(1)\n"
                                "3 : p(1), q(1)\n"
                                "4 : \n";
    std::string rule_string = "b(X) := [$, 3] [D] d(X)\n"
                              "d(X) := q(X) && p(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_recursive_complex() {
    const std::string name = "Recursive Complex";
    std::string stream_string = "1 14 "
                                "1 : q(1), p(1)\n"
                                "2 : p(1)\n"
                                "3 : p(1), q(1)\n"
                                "4 : \n";
    std::string rule_string = "a(X) := b(X) && c(X)\n"
                              "b(X) := [$, 3] [D] d(X)\n"
                              "c(X) := [$, 3] [B] e(X)\n"
                              "e(X) := b(X) && p(X)\n"
                              "d(X) := q(X) && p(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_time_reference_body() {
    const std::string name = "Time Reference Body";
    std::string stream_string = "1 4 "
                                "1 : b(1), b(2)\n"
                                "2 : b(3)\n"
                                "3 : b(4), b(5)\n"
                                "4 : \n";
    std::string rule_string = "a(T, X) := [@, T] b(X) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_time_reference_handb() {
    const std::string name = "Time Reference head and body";
    std::string stream_string = "1 14 "
                                "1 : b(1), b(2)\n"
                                "2 : b(3)\n"
                                "3 : b(4), b(5)\n"
                                "4 : \n";
    std::string rule_string = "[@, T]a(X) := [@, T] b(X) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_time_reference_head() {
    const std::string name = "Time Reference Head";
    std::string stream_string = "1 10 "
                                "1 : b(1,x1), b(2, x1)\n"
                                "2 : b(7, x2)\n"
                                "3 : b(6, x3), b(9, x3)\n"
                                "4 : \n";
    std::string rule_string = "[@,T]a(X) := b(T, X) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_time_reference_recursive() {
    const std::string name = "Time Reference Recursive";
    std::string stream_string =
        "1 10 "
        "1 : a(1,x1), a(2, x1)\n"
        "2 : a(7, x2),c(y2) a(3, x2), a(2, x2), a(9, x2)\n"
        "3 : c(y3)\n"
        "4 : a(5, x4), c(y)\n"
        "6 : c(y6)\n"
        "7 : \n";
    std::string rule_string = "p(X, Y, T) := b(X) && [@,T]c(Y)\n"
                              "[@,T]b(X) := a(T, X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_tuple_window_diamond() {
    const std::string name = "Tuple Window Diamond";
    std::string stream_string = "0 14 "
                                "1 : f(x1)\n"
                                "2 : a(x2)\n"
                                "3 : f(x3)\n"
                                "5 : f(x5), f(y5)\n"
                                "6 : \n"
                                "7 : f(z), f(z), f(z)\n"
                                "8 : f(z)\n"
                                "9 : \n"
                                "10 : \n"
                                "11 : \n"
                                "12 : \n"
                                "13 : \n"
                                "14 : \n";
    std::string rule_string = "u(X) := [#, 1][D]f(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_tuple_window() {
    const std::string name = "Tuple Window";
    std::string stream_string = "0 14 "
                                "0 : \n"
                                // 3 timepoints where f(x) occurs within the
                                // tuple window
                                "1 : f(x)\n"
                                "2 : f(x)\n"
                                "3 : f(x)\n"
                                // 3 timepoints where f(y) occurs, but after
                                // timepoint 5, f(y) slides out of the window
                                "4 : f(y)\n"
                                "5 : f(x), f(y), f(a), a(x), a(y), a(z), a(t)\n"
                                "6 : f(y)\n"
                                // f(z) occurs more than 3 times within the
                                // tuple window but only for 2 consecutive
                                // timepoints
                                "7 : f(z), f(z), f(z)\n"
                                "8 : f(z)\n"
                                "9 : \n"
                                // f(a) occurs at 3 timepoints within the
                                // tuple window, but the 3 timepoints are not
                                // consecutive
                                "10 : f(a)\n"
                                "11 : f(a)\n"
                                "12 : \n"
                                "13 : f(a)\n"
                                "14 : \n";
    std::string rule_string = "u(X) := [#, 3][B]f(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_simple() {
    const std::string name = "Existential Simple";
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_fronteier_var() {
    const std::string name = "Existential Frontier variables";
    std::string stream_string = "1 4 "
                                "1 : p(a, b), p(a, c)\n"
                                "2 : p(a, b), p(c, b)\n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "q(Y, Z) := p(X, Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::SKOLEM);
}

// void test_existential_loop() {
// const std::string name = "Existential Loop";
// std::string stream_string = "1 4 "
//"1 : Bicycle(x1)\n"
//"2 : Bicycle(x2), Bicycle(x3)\n"
//"3 : \n"
//"4 : \n";
// std::string rule_string = "E(v) hasPart(X, v) := Bicycle(X)\n"
//"Wheel(V) := hasPart(X, V) && Bicycle(X)\n"
//"E(w) properPartOf(X, w) := Wheel(X)\n"
//"Bicycle(W) := properPartOf(X, W) && Wheel(X)\n"
//"partOf(X, Y) := properPartOf(X, Y) \n"
//"hasPart(X, Y) := partOf(Y, X) \n"
//"partOf(X, Y) := hasPart(Y, X) \n";

// run(name, stream_string, rule_string);
//}

void test_existential_time_reference_handb() {
    const std::string name = "Existential Time Reference head and body";
    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";
    std::string rule_string = "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
                              "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
                              "[@, T] hasFlat(b, W) := [@, T] exploded(W) "
                              "&& [$, 100] [D] Wheel(W) \n";

    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::RESTRICTED);
}
void test_existential_time_reference_head() {
    const std::string name = "Existential Time Reference head";
    std::string stream_string =
        "1 4 "
        "1 : problem(sg1) \n"
        "2 : willOverheat(sg1, 2), problem(sg2)\n"
        "3 : problem(sg3), problem(sg4)\n"
        "3 : willOverheat(sg3, 4), willOverheat(sg3, 3)\n"
        "4 : \n";
    std::string rule_string =
        "[@, TIME] shutdown(SG, alert) := willOverheat(SG, TIME) "
        "&& [$, 100] [D] problem(SG) \n";

    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::SKOLEM);
}

void test_existential_time_reference_body2() {
    const std::string name = "Existential Time Reference body 2";
    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";
    std::string rule_string = "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
                              "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
                              "hasFlat(b, W, T) := [@, T] exploded(W) && "
                              "[$, 100] [D] Wheel(W) \n";

    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_time_reference_body1() {
    const std::string name = "Existential Time Reference body 1";
    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";
    std::string rule_string = "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
                              "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
                              "hasFlat(b, W, T) := exploded(W) && [$, "
                              "100] [D] [@, T] Wheel(W) \n";

    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_conjunction_two() {
    // TODO Conujunction needs to split input into groundings that can
    // be used by childre. In this case, at timepoint 1, conjunction gets
    // the fact: p(x1,z1,a0,b1), and needs to split it into p(a0, x1) and
    // q(b1,z1).
    const std::string name = "Existential Conjunction two atoms";
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, X) && r(b, Z) := q(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_conjunction_three() {
    const std::string name = "Existential Conjunction three atoms";
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, X) && r(b, Z) && s(a, b) := q(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_restrictive_simple() {
    const std::string name = "Existential Restrictive Simple: at same timpoint";
    std::string stream_string = "1 4 "
                                "1 : q(x1)\n"
                                "2 : s(x2, y2)\n"
                                "3 : q(x3), s(x3, y3)\n"
                                "4 : \n";
    std::string rule_string = "p(X, z)  := q(X)\n"
                              "p (X, Y) := s(X, Y) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_restrictive_conjunction_body() {
    const std::string name = "Existential Restrictive Conjunction Body";
    std::string stream_string = "1 4 "
                                "1 : q(x1)\n"
                                "2 : s(x2, y2)\n"
                                "3 : q(x3), s(x3, y3)\n"
                                "4 : q(x4), s(x4, y4), t(x4, y4, z4)\n";
    std::string rule_string = "r(X, Y, z)  := p(X, Y) && q(X)\n"
                              "p (X, Y) := s(X, Y) \n"
                              "r(X, Y, Z) := t(X, Y, Z)";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_restrictive_conjunction_head_paper() {
    // Example from paper: "Efficient Model Construction for Horn Logic
    // with VLog - System Description" - J. Urbani, M. Krotzsch, I. Dragoste,
    // David Carral - 2018
    // In this example, the computation will terminate after evaluating each
    // rule once. When atemting to re-evaluate the first rule on conclusion:
    // Bicycle(w0), the restrictive chase finds that:
    // hasPart(w0, v0) && Wheel(v0)
    // are in the database, so new null values such as
    // hasPart(w0, v1) && Wheel(v1)
    // are not generated. Thus, the computation terminates due to restrictive
    // chase conditions.
    // See paper for details.
    const std::string name = "Existential Restrictive Conjunction Paper";
    std::string stream_string = "1 2 "
                                "1 : Bicycle(c) \n"
                                "2 : \n";

    std::string rule_string = "hasPart(X, v) && Wheel(v) := Bicycle(X)\n"
                              "properPartOf(X, w) && Bicycle(w) := Wheel(X)\n"
                              "partOf(X, Y) := properPartOf(X, Y)\n"
                              "partOf(Y, X) := hasPart(X, Y)\n"
                              "hasPart(Y, X) := partOf(X, Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_restrictive_conjunction_head_swap() {
    // see test_existential_restrictive_conjunction_head_paper()
    // same exaple, but atoms in head are swaped in the first rule
    const std::string name = "Existential Restrictive Conjunction Head";
    std::string stream_string = "1 2 "
                                "1 : Bicycle(c) \n"
                                "2 : \n";

    std::string rule_string = "Wheel(v) && hasPart(X, v) := Bicycle(X)\n"
                              "properPartOf(X, w) && Bicycle(w) := Wheel(X)\n"
                              "partOf(X, Y) := properPartOf(X, Y)\n"
                              "partOf(Y, X) := hasPart(X, Y)\n"
                              "hasPart(Y, X) := partOf(X, Y)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_restrictive_window() {
    const std::string name = "Existential Restrictive at diferent timpoints";
    std::string stream_string = "1 4 "
                                "1 : s(x1, y1),q(x1)\n"
                                "2 : q(x2)\n"
                                "3 : q(x1)\n"
                                "4 : q(x4)\n";
    std::string rule_string = "p(X, z)  := q(X)\n"
                              "p (X, Y) := [$, 2] [D] s(X, Y) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_existential_indexed_window() {
    const std::string name = "Existential Indexed Window";
    std::string stream_string = "1 4 "
                                "1 : s(x1, y1),q(x1)\n"
                                "2 : q(x2)\n"
                                "3 : q(x1)\n"
                                "4 : q(x4)\n";
    std::string rule_string = "p(X, z)  := q(X)\n"
                              "p (X, Y) := [$, 2] [D] s(X, Y) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::INDEXED);
}

void test_existential_restricted_paper_bmc_eg1() {
    // Example 1 from paper:
    // Benchmarking the Chase - Benedikt et. al.
    const std::string name = "Restricted Chase - Example 1 from "
                             " Benchmarking Chase";
    std::string stream_string = "1 4 "
                                "1 : r(a, b), r(b, b)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "r(X1, y) && A(y) && A(X2) := r(X1, X2) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::RESTRICTED);
}

void test_existential_skolem_paper_bmc_eg1() {
    // Example 1 from paper:
    // Benchmarking the Chase - Benedikt et. al.
    // This program is not exepcted to terminate without Normalizaton of rules
    const std::string name = "Skolem Chase - Example 1 from "
                             " Benchmarking Chase";
    std::string stream_string = "1 4 "
                                "1 : r(a, b), r(b, b)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "r(X1, y) && A(y) && A(X2) := r(X1, X2) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::SKOLEM);
}

void test_existential_skolem_paper_bmc_eg4() {
    // Example 4 from paper:
    // Benchmarking the Chase - Benedikt et. al.
    // This should terminate once skolem chase is fixed such that the parameters
    // of the skolem functions are only the variables occuring in both the
    // body and the head of the rule
    const std::string name = "Skolem Chase - Example 4 from "
                             " Benchmarking Chase";
    std::string stream_string = "1 4 "
                                "1 : r(a, b), r(b, b)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "r(X1, y) && A(y) := r(X1, X2) \n"
                              "A(X2) := r(X1, X2) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::SKOLEM);
}

void test_existential_skolem_complex() {
    // Rule 1 has an unique atom in the head: s(z)
    const std::string name = "Existential Skolem Complex - all nulls";
    std::string stream_string = "1 4 "
                                "1 : p(a1, b1), p(a2, b2)\n"
                                "1 : q(a1), q(b1)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string =
        "s(z) && t(A, B, z) && u(A, z) := p(A, B) && q(B)\n"
        "t(A, B, B) := p(A, B) && q(B) \n"
        "u(A, B) := p(A, B) && q(B) \n"
        "t(A, B, A) := p(A, B) && q(A) \n"
        "u(A, A) := p(A, B) && q(A) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::SKOLEM);
}

void test_existential_skolem_complex2() {
    // Rule 1 has no unique atom in the head
    const std::string name = "Existential Skolem Complex - no nulls";
    std::string stream_string = "1 4 "
                                "1 : p(a1, b1), p(a2, b2)\n"
                                "1 : q(a1), q(b1)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "t(A, B, z) && u(A, z) := p(A, B) && q(B)\n"
                              "t(A, B, B) := p(A, B) && q(B) \n"
                              "u(A, B) := p(A, B) && q(B) \n"
                              "t(A, B, A) := p(A, B) && q(A) \n"
                              "u(A, A) := p(A, B) && q(A) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::SKOLEM);
}

void test_existential_restricted_complex() {
    // Rule 1 has an unique atom in the head: s(z)
    // Rule 1 always generates new null-value because s never occurs in input
    const std::string name = "Existential Restricted Complex - all nulls";
    std::string stream_string = "1 4 "
                                "1 : p(a1, b1), p(a2, b2)\n"
                                "1 : q(a1), q(b1)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string =
        "s(z) && t(A, B, z) && u(A, z) := p(A, B) && q(B)\n"
        "t(A, B, B) := p(A, B) && q(B) \n"
        "u(A, B) := p(A, B) && q(B) \n"
        "t(A, B, A) := p(A, B) && q(A) \n"
        "u(A, A) := p(A, B) && q(A) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::RESTRICTED);
}

void test_existential_restricted_complex2() {
    // Rule 1 has no unique atom in the head
    // Rule 1 never generates new null-value
    const std::string name = "Existential Restricted Complex - no nulls";
    std::string stream_string = "1 4 "
                                "1 : p(a1, b1), p(a2, b2)\n"
                                "1 : q(a1), q(b1)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "t(A, B, z) && u(A, z) := p(A, B) && q(B)\n"
                              "t(A, B, B) := p(A, B) && q(B) \n"
                              "u(A, B) := p(A, B) && q(B) \n"
                              "t(A, B, A) := p(A, B) && q(A) \n"
                              "u(A, A) := p(A, B) && q(A) \n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::RESTRICTED);
}

void test_existential_indexed_complex() {
    // Rule 1 has an unique atom in the head: s(z)
    // Rule 1 always generates new null-value because s never occurs in input
    const std::string name = "Existential Indexed Complex - all nulls";
    std::string stream_string = "1 4 "
                                "1 : p(a1, b1), p(a2, b2)\n"
                                "1 : q(a1), q(b1)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string =
        "s(z) && t(A, B, z) && u(A, z) := p(A, B) && q(B)\n"
        "t(A, B, B) := p(A, B) && q(B) \n"
        "u(A, B) := p(A, B) && q(B) \n"
        "t(A, B, A) := p(A, B) && q(A) \n"
        "u(A, A) := p(A, B) && q(A) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::INDEXED);
}

void test_existential_indexed_complex2() {
    // Rule 1 has no unique atom in the head
    // Rule 1 never generates new null-value
    const std::string name = "Existential Indexed Complex - no nulls";
    std::string stream_string = "1 4 "
                                "1 : p(a1, b1), p(a2, b2)\n"
                                "1 : q(a1), q(b1)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "t(A, B, z) && u(A, z) := p(A, B) && q(B)\n"
                              "t(A, B, B) := p(A, B) && q(B) \n"
                              "u(A, B) := p(A, B) && q(B) \n"
                              "t(A, B, A) := p(A, B) && q(A) \n"
                              "u(A, A) := p(A, B) && q(A) \n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::INDEXED);
}

void test_existential_indexed_simple() {
    // Rule 1 has no unique atom in the head
    // Rule 1 never generates new null-value
    const std::string name = "Indexed Chase Simple";
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    run(name, stream_string, rule_string, laser::util::ChaseAlgorithm::INDEXED);
}

void test_conjunction_head() {
    const std::string name = "Conjunction Head";
    std::string stream_string = "1 2 "
                                "1 : hasPart(x1, y1, z1)\n"
                                "2 : \n";

    std::string rule_string =
        "Bicycle(X) && Wheel(Y) && Wheel(Z) && hasWheel(X,Y) && "
        "hasWheel(X,Z) && partOf(Y,X) && pairOf(Z, Y) && pairOf(Y, Z):= "
        "hasPart(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_conjunction_head_timeref() {
    const std::string name = "Conjunction Head Time Reference";
    std::string stream_string = "1 4 "
                                "1 : hasPart(x1, y1, z1)\n"
                                "2 : scheduleRepair(y1, 3)\n"
                                "3 : \n"
                                "4 : \n";

    std::string rule_string =
        "Bicycle(X) && Wheel(Y) && Wheel(Z) && hasWheel(X,Y) && "
        "hasWheel(X,Z) && partOf(Y,X) && pairOf(Z, Y) && pairOf(Y, Z):= "
        "hasPart(X, Y, Z)\n"
        "hasFlat(X) && [@, T] fixing(Y) := scheduleRepair(Y, T) && "
        "[$, 3] [D]partOf(Y, X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

void test_skolem_simple_no_inertia() {
    const std::string name = "Skolem Simple No Inertia";
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x1, y1, z1)\n"
                                "4 : \n";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::SKOLEM);
}

void test_skolem_inertia_full() {
    const std::string name = "Skolem Inertia Full";
    std::string stream_string = "1 8 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x2, y2, z2)\n"
                                "4 : q(x1, y1, z1)\n"
                                "5 : q(x1, y2, z1)\n"
                                "6 : q(x1, y2, z2)\n"
                                "7 : q(x2, y2, z2)\n"
                                "8 : \n";
    std::string rule_string =
        "p(a, Z, b, X, Z) && [I, a] && [I, b] := q(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::SKOLEM);
}

void test_skolem_inertia_partial() {
    const std::string name = "Skolem Inertia Partial";
    std::string stream_string = "1 7 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x1, y2, z1)\n"
                                "4 : q(x1, y2, z2)\n"
                                "5 : q(x1, y2, z2)\n"
                                "6 : q(x2, y2, z2)\n"
                                "7 : q(x1, y2, z2)\n"
                                "7 : \n";
    std::string rule_string =
        "p(a, Z, b, X, Z) && [I, a] := q(X, Y, Z)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::SKOLEM);
}

int main() {
    //test_skolem_simple_no_inertia(); 
    test_skolem_inertia_full(); 
    //test_skolem_inertia_partial(); 
     //test_simple();
    // test_simple_mix();
    // test_atoms();
    // test_diamond_atom();
    // test_time_window();
    // test_box();
    // test_conjunction_same_variables();
    // test_conjunction_two_variables();
    // test_conjunction_diamond();
    // test_conjunction_box();
    // test_conjunction_corss_variables();
    // test_conjunction_sne();
    // test_conjunction_transitive();
    // test_recursive_simple();
    // test_recursive_complex();
    // test_time_reference_body();
    // test_time_reference_handb();
    // test_time_reference_head();
    // test_time_reference_recursive();
    // test_tuple_window();
    // test_tuple_window_diamond();
    // test_existential_simple();
    // // // // test_existential_loop();
    // test_existential_time_reference_head();
    // test_existential_time_reference_body1();
    // test_existential_time_reference_body2();
    // test_existential_time_reference_handb();
    // test_existential_conjunction_two();
    // test_existential_conjunction_three();
    // test_existential_restrictive_simple();
    // test_existential_restrictive_conjunction_body();
    // test_existential_restrictive_conjunction_head_paper();
    // test_existential_restrictive_conjunction_head_swap();
    // test_existential_restrictive_window();
    // test_conjunction_head();
    // test_conjunction_head_timeref();
    // test_existential_indexed_window();
    // test_existential_skolem_complex();
    // test_existential_skolem_complex2();
    // test_existential_restricted_complex();
    // test_existential_restricted_complex2();
    // test_existential_indexed_simple();
    // test_existential_indexed_complex();
    // test_existential_indexed_complex2();
    // test_existential_restricted_paper_bmc_eg1();
    // test_existential_skolem_paper_bmc_eg4();
    // test_existential_skolem_paper_bmc_eg1();
    // test_queries();
    //test_existential_fronteier_var();
    //test_acyclicity_simple();
    //test_acyclicity_obvious_cycle();
    //test_acyclicity_obvious_cycle_with_time();
    //test_acyclicity_only_first_timepoint();
}
