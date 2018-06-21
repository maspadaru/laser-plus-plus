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

    auto simple_data_reader = SimpleReader();
    auto simple_rule_reader = SimpleReader();
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