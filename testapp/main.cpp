#include <iostream>

#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <rule/rule.h>
#include <program/program.h>

#include "simple_reader.h"
#include "simple_parser.h"
#include "simple_writer.h"
#include "simple_io_manager.h"


int main() {
    std::string stream_string =
            "1 14 "
            "1 : \n"
            "2 : c(x2, y2)\n"
            "2 : d(x2, z2)\n"
            "4 : \n"
            "5 : \n"
            "5 : e(x5, y5), e(z5, x5), e(x5, y5)\n"
            "5 : e(x5, y5)"
            "6 : a(x6, y6, z6)\n"
            "7 : \n"
            "8 : a(x8, y8, z8), b(y8, z8)\n"
            "9 : \n"
            "10 : a(x10, y10, z10)\n"
            "11 : f(a11)\n"
            "12 : \n"
            "13 : \n"
            "14 : \n";

    std::string rule_string =
            "q(X, Y, Z) :- a(X, Y, Z)\n"
            "r(Y) :- c(X, Y)\n"
            "s(X) :- e(X, X)\n"
            "t(Y, X) :- d(X, Y)\n"
            "u(X, X) :- f(X)\n";


    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    program.set_start_time(1);

    while (!program.is_done()) {
        program.evaluate();
    }


    std::cerr << " ============== DONE! ============== " << std::endl;
}
