#include <iostream>

#include <reasoner/sequential_reasoner.h>
#include <rule/default_rule_reader.h>

#include "simple_io_manager.h"

void run(std::string const &name, std::string const &stream_string,
         std::string const &rule_string) {
    std::cout << std::endl;
    std::cout << " Test: " << name << std::endl;
    std::cout << " =================================== " << std::endl;
    auto simple_io_manager = SimpleIOManager(stream_string);
    auto rule_reader = laser::rule::DefaultRuleReader(rule_string);
    auto reasoner =
        laser::reasoner::SequentialReasoner(&rule_reader, &simple_io_manager);

    reasoner.start();

    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_simple() {
    const std::string name = "Simple";
    std::string stream_string = "1 4 "
                                "1 : a(x1, y1, z1)\n"
                                "2 : a(x2, y2, z2)\n"
                                "3 : a(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(X, Y, Z) := a(X, Y, Z)\n";
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
}

void test_diamond() {
    const std::string name = "Diamond";
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
}

void test_recursive() {
    const std::string name = "Recursive";
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
    run(name, stream_string, rule_string);
}

void test_exact_time_body() {
    const std::string name = "Time Reference Body";
    std::string stream_string = "1 14 "
                                "1 : b(1), b(2)\n"
                                "2 : b(3)\n"
                                "3 : b(4), b(5)\n"
                                "4 : \n";
    std::string rule_string = "a(T, X) := [@, T] b(X) \n";
    run(name, stream_string, rule_string);
}

void test_exact_time_handb() {
    const std::string name = "Time Reference head and body";
    std::string stream_string = "1 14 "
                                "1 : b(1), b(2)\n"
                                "2 : b(3)\n"
                                "3 : b(4), b(5)\n"
                                "4 : \n";
    std::string rule_string = "[@, T]a(X) := [@, T] b(X) \n";
    run(name, stream_string, rule_string);
}

void test_exact_time_head() {
    const std::string name = "Time Reference Head";
    std::string stream_string = "1 10 "
                                "1 : b(1,x1), b(2, x1)\n"
                                "2 : b(7, x2)\n"
                                "3 : b(6, x3), b(9, x3)\n"
                                "4 : \n";
    std::string rule_string = "[@,T]a(X) := b(T, X) \n";
    run(name, stream_string, rule_string);
}

void test_exact_time_recursive() {
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
    run(name, stream_string, rule_string);
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
    run(name, stream_string, rule_string);
}

int main() {
    test_simple();
    test_atoms();
    test_diamond();
    test_time_window();
    test_box();
    test_conjunction_same_variables();
    test_conjunction_two_variables();
    test_conjunction_diamond();
    test_conjunction_box();
    test_conjunction_corss_variables();
    test_recursive();
    test_exact_time_body();
    test_exact_time_handb();
    test_exact_time_head();
    test_exact_time_recursive();
    test_tuple_window();
}
