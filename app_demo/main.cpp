#include <iostream>
#include <fstream>

#include <core/reasoner.h>
#include <util/chase_algorithm.h>
#include <service/service_manager.h>

#include "file_io_manager.h"
#include "simple_rule_reader.h"

void run(laser::util::ChaseAlgorithm chase_algorithm,
         std::string const &program_path, std::string const &stream_path,
         std::string const &output_path, std::string const &request_in_path,
         std::string const &request_out_path) {
    std::cout << " LASER++ " << std::endl;

    // TODO change FileIOManager to read all tuples in input
    // have no timelimit
    // wait for input untill input is available
    auto file_io_manager =
        FileIOManager(stream_path, output_path, 1, 4,
                      3, true);

    std::ifstream in;
    in.open(request_in_path);
    std::ofstream out;
    out.open(request_out_path);

    auto sm = laser::service::ServiceManager(in, out);

    auto rule_reader = SimpleRuleReader(program_path);
    std::cout << " Reasoning... " << std::endl;
    auto reasoner = laser::core::Reasoner(&rule_reader, &file_io_manager,
                                          chase_algorithm, &sm);
    reasoner.start();
    std::cout << " Done! " << std::endl;
    in.close();
    out.close();
}

int main(int argc, char **argv) {
    // if (argc < 5) {
    // std::cerr << "Usage: demoapp CHASE_ALGORITHM=(S/R/I) PROGRAM_PATH "
    //<< std::endl;
    // return 1;
    //}
    // std::string chase_algorithm_str(argv[1]);
    // std::string program_path(argv[2]);
    // std::string stream_path(argv[3]);
    // std::string output_path(argv[4]);

    std::string program_path = "/home/mike/samples/program_simple.laser";
    std::string stream_path = "/home/mike//samples/simple.stream";
    std::string request_in_path = "/home/mike//samples/simple_query.txt";
    std::string output_path = "/home/mike//output_stream.txt";
    std::string request_out_path = "/home/mike//output_query.txt";;

    auto chase_algorithm = laser::util::ChaseAlgorithm::OBLIVIOUS;
    // if (chase_algorithm_str == "S") {
    // chase_algorithm = laser::util::ChaseAlgorithm::SKOLEM;
    //} else if (chase_algorithm_str == "R") {
    // chase_algorithm = laser::util::ChaseAlgorithm::RESTRICTED;
    //} else if (chase_algorithm_str == "I") {
    // chase_algorithm = laser::util::ChaseAlgorithm::INDEXED;
    //}

    run(chase_algorithm, program_path, stream_path, output_path,
        request_in_path, request_out_path);
}
