#include <chrono>
#include <iostream>

#include <core/reasoner.h>
#include <example/example_rule_reader.h>
#include "file_io_manager.h"

const uint64_t BUILD_NUMBER = 1;

uint64_t end_time = 10000;
uint64_t NFACTS = 100;

void run(uint64_t end_time, int facts_per_timepoint,
         std::string const &stream_path, std::string const &output_path,
         bool is_output_enabled, std::string const &rules) {
    uint64_t start_time = 0;
    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto rule_reader = laser::example::ExampleRuleReader(rules);
    auto reasoner = laser::core::Reasoner(&rule_reader, &file_io_manager);

    auto clock_start = std::chrono::high_resolution_clock::now();
    reasoner.start();
    auto clock_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> clock_elapsed = clock_end - clock_start;
    std::cout << "Total seconds: " << clock_elapsed.count() / 1000 << std::endl;
    std::cout << std::endl;
}

void run_atom(uint64_t end_time, int facts_per_timepoint, int window_size,
              std::string const &stream_path, std::string const &output_path,
              bool is_output_enabled) {
    std::string rules = ""
                        "t(X, Y) := d(X, Y)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_atom_window(uint64_t end_time, int facts_per_timepoint,
                     int window_size, std::string const &stream_path,
                     std::string const &output_path, bool is_output_enabled) {
    std::string rules =
        "t(X, Y) := [$, " + std::to_string(window_size) + "] d(X, Y)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_diamond(uint64_t end_time, int facts_per_timepoint, int window_size,
                 std::string const &stream_path, std::string const &output_path,
                 bool is_output_enabled) {
    std::string rules =
        "t(X, Y) := [$, " + std::to_string(window_size) + "] [D]d(X, Y)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_conjunction(uint64_t end_time, int facts_per_timepoint,
                     int window_size, std::string const &stream_path,
                     std::string const &output_path, bool is_output_enabled) {
    std::string rules = "t(X, Y) := d(X, Y) && f(X)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_box(uint64_t end_time, int facts_per_timepoint, int window_size,
             std::string const &stream_path, std::string const &output_path,
             bool is_output_enabled) {
    std::string rules =
        "t(X) := [$, " + std::to_string(window_size) + "][B]f(X)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_time_reference(uint64_t end_time, int facts_per_timepoint,
                        int window_size, std::string const &stream_path,
                        std::string const &output_path,
                        bool is_output_enabled) {
    std::string rules = "t(X, Y, T) := [@, T]d(X, Y)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_multi_atom(uint64_t end_time, int facts_per_timepoint, int window_size,
                    std::string const &stream_path,
                    std::string const &output_path, bool is_output_enabled) {
    std::string rules = ""
                        "q(X, Y, Z) := a(X, Y, Z)\n"
                        "r(Y) := c(X, Y)\n"
                        "t(Y, X) := d(X, Y)\n"
                        "u(X, X) := f(X)\n"
                        "v(X, Y, X, Y) := e(X, Y)\n";
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

void run_multi_all(uint64_t end_time, int facts_per_timepoint, int window_size,
                   std::string const &stream_path,
                   std::string const &output_path, bool is_output_enabled) {
    std::string rules = "r(Y, Z) := c(X, Y) && d(Y,Z)\n"
                        "t(X) := [$, " +
                        std::to_string(window_size) +
                        "] u(X, X)\n"
                        "u(X, X) := [$, " +
                        std::to_string(window_size) +
                        "][B]f(X)\n"
                        "v(X, Y, X, Y) := e(X, Y)\n"
        //"q(X, Y, Z, T) := [@, T]a(X, Y, Z)\n"
        ;
    run(end_time, facts_per_timepoint, stream_path, output_path,
        is_output_enabled, rules);
}

int main(int argc, char **argv) {
    if (argc < 6) {
        std::cerr << "Usage: benchapp TEST_NAME END_TIME FACTS_PER_TIMEPOINT "
                     "WINDOW_SIZE INPUT_PATH [OUTPUT_PATH]"
                  << std::endl;
        return 1;
    }
    std::string test_name(argv[1]);
    std::string end_time_str(argv[2]);
    std::string num_facts_str(argv[3]);
    std::string win_size_str(argv[4]);
    std::string stream_path(argv[5]);
    std::string output_path;
    bool is_output_enabled = false;
    if (argc == 7) {
        std::string output_path(argv[6]);
        is_output_enabled = true;
    }

    uint64_t end_time = std::stoull(end_time_str);
    int num_facts = std::stoi(num_facts_str);
    int win_size = std::stoi(win_size_str);

    std::cout << "Starting run: " << test_name << std::endl
              << "timepoints: " << end_time
              << ",  facts per timepoint: " << num_facts
              << ",  window size: " << win_size << std::endl;

    if (test_name == "atom") {
        run_atom(end_time, num_facts, win_size, stream_path, output_path,
                 is_output_enabled);
    } else if (test_name == "atom_window") {
        run_atom_window(end_time, num_facts, win_size, stream_path, output_path,
                        is_output_enabled);
    } else if (test_name == "diamond") {
        run_diamond(end_time, num_facts, win_size, stream_path, output_path,
                    is_output_enabled);
    } else if (test_name == "conjunction") {
        run_conjunction(end_time, num_facts, win_size, stream_path, output_path,
                        is_output_enabled);
    } else if (test_name == "box") {
        run_box(end_time, num_facts, win_size, stream_path, output_path,
                is_output_enabled);
    } else if (test_name == "tref") {
        run_time_reference(end_time, num_facts, win_size, stream_path,
                           output_path, is_output_enabled);
    } else if (test_name == "multiatom") {
        run_multi_atom(end_time, num_facts, win_size, stream_path, output_path,
                       is_output_enabled);
    } else if (test_name == "multiall") {
        run_multi_all(end_time, num_facts, win_size, stream_path, output_path,
                      is_output_enabled);
    } else {
        std::cout << "Invalid test name!" << std::endl;
    }
}
