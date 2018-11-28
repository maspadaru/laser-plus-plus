#include <iostream>

#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "file_io_manager.h"
#include "file_parser.h"
#include "file_reader.h"
#include "file_writer.h"

void run_laser() {
    std::cout << std::endl;
    std::cout << " ============= LASERPP ================== " << std::endl;

    std::string rule_string = "q(X, Y, Z) := a(X, Y, Z)\n"
                              "r(Y) := c(X, Y)\n"
                              "t(Y, X) := d(X, Y)\n"
                              "u(X, X) := f(X)\n"
                              "v(X, Y, X, Y) := e(X, Y)\n";

    std::string stream_path = "stream_file.txt";
    std::string output_path = "output.txt";
    uint64_t start_time = 0;
    uint64_t end_time = 14;

    auto file_io_manager = FileIOManager(stream_path, output_path, start_time, end_time);
    auto program = laser::program::Program(rule_string, &file_io_manager);
    program.set_start_time(0);

    while (!program.is_done()) {
        program.evaluate();
    }
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}


int main() {
    run_laser();
}
