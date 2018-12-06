#include <ctime>
#include <iostream>

#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "file_io_manager.h"
#include "file_parser.h"
#include "file_reader.h"
#include "file_writer.h"

const uint64_t BUILD_NUMBER = 2; 
const uint64_t END_TIME = 1000000;
const uint64_t NFACTS = 1;
const bool OUTPUT = false;

void run_single() {
    std::cout << "Starting run: Single Atom " << std::endl;

    std::string rules = ""
                        "t(X, Y) := d(X, Y)\n"
                        ;

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_atom_single.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

void run_diamond() {
    std::cout << "Starting run: Diamond " << std::endl;

    std::string rules = "t(X, Y) := [$, 5] [D]d(X, Y)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_diamond.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

void run_conjunction() {
    std::cout << "Starting run: Conjunction " << std::endl;

    std::string rules = "t(X, Y) := d(X, Y) && f(X)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_conjunction.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

void run_box() {
    std::cout << "Starting run: Box " << std::endl;

    std::string rules = "t(X) := [$, 3][B]f(X)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_box.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

void run_exact_time() {
    std::cout << "Starting run: Exact Time " << std::endl;

    std::string rules = "t(X, Y, T) := [@, T]d(X, Y)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_timeref.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

void run_atoms() {
    std::cout << "Starting run: Atoms " << std::endl;

    std::string rules = ""
                        "q(X, Y, Z) := a(X, Y, Z)\n"
                        "r(Y) := c(X, Y)\n"
                        "t(Y, X) := d(X, Y)\n"
                        "u(X, X) := f(X)\n"
                        "v(X, Y, X, Y) := e(X, Y)\n"
                        ;

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_atoms.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

void run_all() {
    std::cout << "Starting run: All " << std::endl;

    std::string rules = "r(Y, Z) := c(X, Y) && d(Y,Z)\n"
                            "t(X) := [$, 5] u(X, X)\n"
                            "u(X, X) := [$, 3][B]f(X)\n"
                            "v(X, Y, X, Y) := e(X, Y)\n"
                             "q(X, Y, Z, T) := [@, T]a(X, Y, Z)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp.txt";
    uint64_t start_time = 0;
    uint64_t end_time = END_TIME;
    int facts_per_timepoint = NFACTS;
    bool is_output_enabled = OUTPUT;

    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto program = laser::program::Program(rules, &file_io_manager);
    program.set_start_time(0);

    clock_t begin = clock();

    while (!program.is_done()) {
        program.evaluate();
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time (sec): " << elapsed_secs << std::endl
              << std::endl
              << std::endl;
}

int main() { 
    std::cout 
        << " build: " << BUILD_NUMBER << std::endl 
        << " timepoints: " << END_TIME << std::endl 
        << " facts per timepoint: " << NFACTS << std::endl
       << "output enabled: "<< OUTPUT << std::endl << std::endl;
    run_single();
    run_diamond(); 
    run_conjunction(); 
    run_box(); 
    //run_exact_time(); 
    //run_atoms(); 
    //run_all(); 
}
