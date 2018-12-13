#include <ctime>
#include <iostream>
#include <string.h>

#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "file_io_manager.h"
#include "file_parser.h"
#include "file_reader.h"
#include "file_writer.h"

const uint64_t BUILD_NUMBER = 1; 
const bool OUTPUT = false;

uint64_t end_time = 10000;
uint64_t NFACTS = 100;

void run_atom(uint64_t end_time, int facts_per_timepoint, int window_size) {

    std::string rules = ""
                        "t(X, Y) := d(X, Y)\n"
                        ;

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_atom_single.txt";
    uint64_t start_time = 0;
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
              << std::endl;
}

void run_diamond(uint64_t end_time, int facts_per_timepoint, int window_size) {

    std::string rules = "t(X, Y) := [$, " + std::to_string(window_size) +  
                                    "] [D]d(X, Y)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_diamond.txt";
    uint64_t start_time = 0;
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

void run_conjunction(uint64_t end_time, int facts_per_timepoint, int window_size) {

    std::string rules = "t(X, Y) := d(X, Y) && f(X)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_conjunction.txt";
    uint64_t start_time = 0;
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

void run_box(uint64_t end_time, int facts_per_timepoint, int window_size) {

    std::string rules = "t(X) := [$, " + std::to_string(window_size) +  
                                    "][B]f(X)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_box.txt";
    uint64_t start_time = 0;
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

void run_time_reference(uint64_t end_time, int facts_per_timepoint, int window_size) {

    std::string rules = "t(X, Y, T) := [@, T]d(X, Y)\n";

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp_timeref.txt";
    uint64_t start_time = 0;
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

void run_multi_atom(uint64_t end_time, int facts_per_timepoint, int window_size) {

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

void run_multi_all(uint64_t end_time, int facts_per_timepoint, int window_size) {

    std::string rules = "r(Y, Z) := c(X, Y) && d(Y,Z)\n"
                            "t(X) := [$, " + std::to_string(window_size) +  
                                    "] u(X, X)\n"
                            "u(X, X) := [$, " + std::to_string(window_size) +  
                                    "][B]f(X)\n"
                            "v(X, Y, X, Y) := e(X, Y)\n"
                             //"q(X, Y, Z, T) := [@, T]a(X, Y, Z)\n"
                             ;

    std::string stream_path = "/home/mike/stream_file.txt";
    std::string output_path = "/home/mike/out_cpp.txt";
    uint64_t start_time = 0;
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

int main(int argc, char **argv) { 
    if (argc < 4) {
        std::cerr << "Usage: benchapp TEST_NAME END_TIME FACTS_PER_TIMEPOINT WINDOW_SIZE" 
            << std::endl;
        return 1;
    }
    std::string test_name(argv[1]);
    std::string end_time_str(argv[2]);
    std::string num_facts_str(argv[3]);
    std::string win_size_str(argv[4]);

    uint64_t end_time = std::stoull(end_time_str);
    int num_facts = std::stoi(num_facts_str);
    int win_size = std::stoi(win_size_str);

    std::cout 
       << "Starting run: " << test_name << std::endl
       << "timepoints: " << end_time 
       << ",  facts per timepoint: " << num_facts 
       << ",  window size: " << win_size << std::endl;

    if (test_name == "atom") {
        run_atom(end_time, num_facts, win_size);
    } else if (test_name == "diamond") {
        run_diamond(end_time, num_facts, win_size); 
    } else if (test_name == "conjunction") {
        run_conjunction(end_time, num_facts, win_size); 
    } else if (test_name == "box") {
        run_box(end_time, num_facts, win_size); 
    } else if (test_name == "tref") {
        run_time_reference(end_time, num_facts, win_size); 
    } else if (test_name == "multiatom") {
        run_multi_atom(end_time, num_facts, win_size); 
    } else if (test_name == "multiall") {
        run_multi_all(end_time, num_facts, win_size); 
    }
}
