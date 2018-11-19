#include <iostream>

#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "simple_io_manager.h"
#include "simple_parser.h"
#include "simple_reader.h"
#include "simple_writer.h"

/* TODO
 *
 * FEATURES:
 * - Implement use of Tuple Counter
 * - Program should not set_start_time(), it should get current time from the
 *      stream
 *
 *
 *
 *
 * BUGS:
 * - Eliminate duplicates in Grounding Table. E.g.: test_atoms, time 5 sould
 *   otuput v(x5, y5, x5, y5) only once
 *
 *
 *
 *
 * OPTIMIZATIONS:
 *
 *
 *
 *
 * REFACTORING:
 * - Refactor Program::evaluate() & Program::eval() [P46-L10]
 * - Structure project as a library.
 * - Make use of Unit Tests
 * - Move debug_print() functions to new LOG module [P52-L10]
 *
 */

void test_simple() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Simple";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

    std::string stream_string = "1 4 "
                                "1 : a(x1, y1, z1)\n"
                                "2 : a(x2, y2, z2)\n"
                                "3 : a(x3, y3, z3)\n"
                                "4 : \n";

    std::string rule_string = "p(X, Y, Z) := a(X, Y, Z)\n";

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_atoms() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Atoms";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_diamond() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Diamond";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_time_window() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Time Window";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_box() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Box";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    std::string rule_string = "q(X) := [B] f(X)\n"
                              "r(Y,X) := [$, 3][B]c(X, Y)\n";

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_conjunction_same_variables() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Conjunction same variables";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_conjunction_two_variables() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Conjunction two variables";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_conjunction_diamond() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Conjunction Diamond";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_conjunction_box() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Conjunction Box";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_conjunction_corss_variables() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Conjunction cross variables";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void test_recursive() {
    std::cout << std::endl;
    const std::string TEST_NAME = "Recursive";
    std::cout << " Test: " << TEST_NAME << std::endl;
    std::cout << " =================================== " << std::endl;

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


    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
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
}
