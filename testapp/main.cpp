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

void test_atoms() {
    std::cout << std::endl;
    const std::string test_name = "Atoms";
    std::cout << " Test: " << test_name << std::endl;
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

int main() { test_atoms(); }
