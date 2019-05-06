#include <chrono>
#include <iostream>

#include "file_io_manager.h"
#include <core/reasoner.h>
#include <example/example_rule_reader.h>
#include <util/chase_algorithm.h>

const uint64_t BUILD_NUMBER = 1;

uint64_t end_time = 10000;
uint64_t NFACTS = 100;

std::string get_rules(std::string const &program_id, int win_size) {
    std::string rules;
    if (program_id == "p1") {
        rules = "r(X, Y) := p(X, Y)\n";
    } else if (program_id == "p2") {
        rules = "r(X, X) := p(X)\n";
    } else if (program_id == "p3") {
        rules = "r(X, Y) := p(Y,X)\n";
    } else if (program_id == "p4") {
        rules = "r(X) := p(X,Y)\n";
    } else if (program_id == "p5") {
        rules = "r(X, Y) := p(X,Y) && q(X)\n";
    } else if (program_id == "p6") {
        rules = "r(X, Y, Z) := p(X,Y) && q(Z)\n";
    } else if (program_id == "p7") {
        rules = "r(X) := [$, " + std::to_string(win_size) + "] [D]p(X)\n";
    } else if (program_id == "p8") {
        rules = "r(X) := [$, " + std::to_string(win_size) + "] [B]p(X)\n";
    } else if (program_id == "p9") {
        rules = ""
                "a(X) := p(X)\n"
                "b(X) := q(X)\n";
    } else if (program_id == "p10") {
        rules = ""
                "a(X) := p(X)\n"
                "b(X) := q(X)\n"
                "c(X) := r(X)\n"
                "d(X) := s(X)\n";
    } else if (program_id == "p11") {
        rules = ""
                "a(X) := p(X)\n"
                "b(X) := q(X)\n"
                "c(X) := r(X)\n"
                "d(X) := s(X)\n"
                "e(X) := t(X)\n"
                "f(X) := u(X)\n"
                "g(X) := v(X)\n"
                "h(X) := w(X)\n";
    } else if (program_id == "p12") {
        rules = "r(A, B) := p(A, B)\n";
    } else if (program_id == "p13") {
        rules = "r(A, B, C, D) := p(A, B, C, D)\n";
    } else if (program_id == "p14") {
        rules = "r(A, B, C, D, E, F, G, H) := p(A, B, C, D, E, F, G, H)\n";
    } else if (program_id == "p15") {
        rules = "r(X) := p(X)\n";
    } else if (program_id == "p16") {
        rules = "q(X, a) && r(a, b, Y) := p(X, Y)\n";
    } else if (program_id == "p17") {
        rules = ""
                "a(X, a) := p(X)\n"
                "b(X, b) := q(X)\n";
    } else if (program_id == "p18") {
        rules = ""
                "a(X, a) := p(X)\n"
                "b(X, b) := q(X)\n"
                "c(X, c) := r(X)\n"
                "d(X, d) := s(X)\n";
    } else if (program_id == "p19") {
        rules = ""
                "a(X, a) := p(X)\n"
                "b(X, b) := q(X)\n"
                "c(X, c) := r(X)\n"
                "d(X, d) := s(X)\n"
                "e(X, e) := t(X)\n"
                "f(X, f) := u(X)\n"
                "g(X, g) := v(X)\n"
                "h(X, h) := w(X)\n";
    } else if (program_id == "p20") {
        rules = "p(X, Z) := p(X, Y) && p(Y, Z)\n";
    } else if (program_id == "p21") {
        rules = "r(A, B) := [$, " + std::to_string(win_size) + "] [B]p(A, B)\n";
    } else if (program_id == "p22") {
        rules = "r(A, B, C, D) := [$, " + std::to_string(win_size) +
                "] [B]p(A, B, C, D)\n";
    } else if (program_id == "p23") {
        rules = "r(A, B, C, D, E, F, G, H) := [$, " + std::to_string(win_size) +
                "] [B]p(A, B, C, D, E, F, G, H)\n";
    }
    return rules;
}

void run(uint64_t end_time, int facts_per_timepoint,
         laser::util::ChaseAlgorithm chase_algorithm,
         std::string const &stream_path, std::string const &output_path,
         bool is_output_enabled, std::string const &rules) {
    uint64_t start_time = 0;
    auto file_io_manager =
        FileIOManager(stream_path, output_path, start_time, end_time,
                      facts_per_timepoint, is_output_enabled);
    auto rule_reader = laser::example::ExampleRuleReader(rules);
    auto reasoner =
        laser::core::Reasoner(&rule_reader, &file_io_manager, chase_algorithm);

    auto clock_start = std::chrono::high_resolution_clock::now();
    reasoner.start();
    auto clock_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> clock_elapsed =
        clock_end - clock_start;
    std::cout << "Total seconds: " << clock_elapsed.count() / 1000 << std::endl;
    std::cout << "************************************************************"
              << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 6) {
        std::cerr << "Usage: benchapp TEST_NAME END_TIME "
                     "FACTS_PER_TIMEPOINT "
                     "WINDOW_SIZE CHASE_ALGORITHM=(O / S / R) INPUT_PATH "
                     "[OUTPUT_PATH]"
                  << std::endl;
        return 1;
    }
    std::string program_id(argv[1]);
    std::string end_time_str(argv[2]);
    std::string fact_count_str(argv[3]);
    std::string win_size_str(argv[4]);
    std::string chase_algorithm_str(argv[5]);
    std::string stream_path(argv[6]);
    std::string output_path;
    bool is_output_enabled = false;
    if (argc == 8) {
        std::string output_path(argv[7]);
        is_output_enabled = true;
    }

    uint64_t end_time = std::stoull(end_time_str);
    int fact_count = std::stoi(fact_count_str);
    int win_size = std::stoi(win_size_str);

    auto chase_algorithm = laser::util::ChaseAlgorithm::OBLIVIOUS;
    if (chase_algorithm_str == "S") {
        chase_algorithm = laser::util::ChaseAlgorithm::SKOLEM;
    } else if (chase_algorithm_str == "R") {
        chase_algorithm = laser::util::ChaseAlgorithm::RESTRICTED;
    }

    std::string rules = get_rules(program_id, win_size);

    std::cout << "Run: " << program_id << " - chase: " << chase_algorithm_str
              << std::endl
              << "timepoints: " << end_time << ",  fact_count: " << fact_count
              << ",  win_size: " << win_size << std::endl;

    run(end_time, fact_count, chase_algorithm, stream_path, output_path,
        is_output_enabled, rules);
}
