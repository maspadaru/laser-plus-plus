//
// Created by mike on 6/19/18.
//

#include <iostream>
#include <simple_reader.h>
#include <simple_parser.h>
#include <input/input_manager.h>
#include <program/program.h>
#include <simple_formatter.h>
#include <simple_writer.h>

int main() {

    std::string stream_string =
            "1 14"
            "1 : \n"
            "2 : c(x4, z)\n"
            "2 : d(x5, z)\n"
            "4 : \n"
            "5 : \n"
            "5 : e(x7, y), e(z, x7), e(x9, y)\n"
            "5 : e(x1, y)"
            "6 : a(x1, y)\n"
            "7 : \n"
            "8 : a(x2, y), b(y, z)\n"
            "9 : \n"
            "10 : a(x3, y)\n"
            "11 : \n"
            "12 : \n"
            "13 : \n"
            "14 : \n";

    std::string rule_string =
            "r1 : q(X, Y, Z) :- a(X, Y) and b(Y, Z)\n"
            "r2 : r(X, Y) :- c(X, Y)\n"
            "r3 : s(Z, Y) :- e(X, Y) and e(Z, X)\n"
            "r4 : t(X, Y) :- a(X, Y)\n";

    auto simple_data_reader = SimpleReader();
    simple_data_reader.set_source(stream_string);
    auto simple_rule_reader = SimpleReader();
    simple_rule_reader.set_source(rule_string);
    auto simple_parser = SimpleParser();
    auto input_manager = laser::input::InputManager();
    input_manager.initialize_rule_reader(&simple_rule_reader, &simple_parser);
    input_manager.initialize_stream_reader(&simple_data_reader, &simple_parser);

    auto simple_formatter = SimpleFormatter();
    auto simple_writer = SimpleWriter();
    auto output_manager = laser::output::OutputManager();
    output_manager.initialize_output_manager(&simple_writer, &simple_formatter);

    auto program = laser::program::Program(input_manager, output_manager);

    while (!program.is_done()) {
        program.evaluate();
    }








    std::cout << "Hello, " << "!" << std::endl;
}
