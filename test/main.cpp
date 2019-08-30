#include <iostream>

#include <acyclicity/naive_smfa.h>
#include <core/reasoner.h>
#include <example/example_io_manager.h>
#include <rule/rule_parser.h>
#include <util/chase_algorithm.h>

void run(std::string const &name, std::string const &stream_string,
         std::string const &rule_string,
         laser::util::ChaseAlgorithm chase_algorithm) {
    std::cout << std::endl;
    std::cout << " Test: " << name << std::endl;
    std::cout << " =================================== " << std::endl;
    // It might not be very intuitive, but chase_algorithm needs to be set
    // before parsing the rules !!!
    // As a general rule set the settings before initializing anything else.
    // I should make this more obvious somehow...
    laser::util::Settings::get_instance().set_chase_algorithm(chase_algorithm);
    auto example_io_manager = laser::example::ExampleIOManager(stream_string);
    auto rule_parser = laser::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto reasoner = laser::core::Reasoner(&rule_vector, &example_io_manager);
    reasoner.start();
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl << std::endl;
}

void run_acyclicity_test(std::string const &name,
                         std::string const &rule_string, bool expected) {
    bool acyclicity_result;
    std::cout << std::endl;
    std::cout << " Acyclicity Test: " << name << std::endl;
    laser::util::Settings::get_instance().set_chase_algorithm(
        laser::util::ChaseAlgorithm::SKOLEM);
    auto rule_parser = laser::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto naive_smfa = laser::acyclicity::NaiveSMFA(rule_vector);
    acyclicity_result = naive_smfa.has_terminating_chase();
    std::cout << " Naive_SMFA returned: " << acyclicity_result
              << " expected: " << expected << ";" << std::endl;
    std::cout << " =================================== " << std::endl;
    std::cout << std::endl;
}

void test_acyclicity_simple() {
    const std::string name = "Acyclicity Simple";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    bool expected_has_terminating_chase = true;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_evaluation_one() {
    const std::string name = "Evaluation Test One";
    std::string stream_string = "1 14 "
                                "1 : q(1), p(1)\n"
                                "2 : p(1)\n"
                                "3 : p(1), q(1)\n"
                                "4 : \n";
    std::string rule_string = "a(X) := b(X) && c(X)\n"
                              "b(X) := [$, 3] [D] d(X)\n"
                              "c(X) := [$, 3] [B] e(X)\n"
                              "e(X) := b(X) && p(X)\n"
                              "d(X) := q(X) && p(X)\n";
    run(name, stream_string, rule_string,
        laser::util::ChaseAlgorithm::OBLIVIOUS);
}

int main() {
    test_evaluation_one(); 
}
