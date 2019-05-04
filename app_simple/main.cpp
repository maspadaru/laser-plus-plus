#include <iostream>

#include <core/reasoner.h>
#include <util/chase_algorithm.h>

#include "simple_io_manager.h"
#include "simple_rule_reader.h"

void run(laser::util::ChaseAlgorithm chase_algorithm,
         std::string const &program_path, std::string const &stream_path,
         std::string const &output_path) {
    std::cout << " LASER++ " << std::endl;
    auto simple_io_manager = SimpleIOManager(stream_path, output_path);
    auto rule_reader = SimpleRuleReader(program_path);
    std::cout << " Reasoning... " << std::endl;
    auto reasoner = laser::core::Reasoner(&rule_reader, &simple_io_manager,
                                          chase_algorithm);
    reasoner.start();
    std::cout << " Done! " << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 5) {
        std::cerr << "Usage: simpleapp CHASE_ALGORITHM=(S/R) PROGRAM_PATH "
                     "INPUT_PATH OUTPUT_PATH  "
                  << std::endl;
        return 1;
    }
    std::string chase_algorithm_str(argv[1]);
    std::string program_path(argv[2]);
    std::string stream_path(argv[3]);
    std::string output_path(argv[4]);

    auto chase_algorithm = laser::util::ChaseAlgorithm::OBLIVIOUS;
    if (chase_algorithm_str == "S") {
        chase_algorithm = laser::util::ChaseAlgorithm::SKOLEM;
    } else if (chase_algorithm_str == "R") {
        chase_algorithm = laser::util::ChaseAlgorithm::RESTRICTED;
    }

    run(chase_algorithm, program_path, stream_path, output_path);
}
