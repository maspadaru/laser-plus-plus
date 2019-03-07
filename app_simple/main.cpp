#include <iostream>

#include <core/reasoner.h>

#include "simple_io_manager.h"
#include "simple_rule_reader.h"

void run(std::string const &program_path, std::string const &stream_path,
         std::string const &output_path) {
    std::cout << " LASER++ " << std::endl;
    auto simple_io_manager = SimpleIOManager(stream_path, output_path);
    auto rule_reader = SimpleRuleReader(program_path);
    std::cout << " Reasoning... " << std::endl;
    auto reasoner =
        laser::core::Reasoner(&rule_reader, &simple_io_manager);
    reasoner.start();
    std::cout << " Done! " << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cerr << "Usage: simpleapp PROGRAM_PATH INPUT_PATH OUTPUT_PATH"
                  << std::endl;
        return 1;
    }
    std::string program_path(argv[1]);
    std::string stream_path(argv[2]);
    std::string output_path(argv[3]);
    run(program_path, stream_path, output_path);
}
