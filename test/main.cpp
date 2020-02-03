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
    auto reasoner = laser::core::Reasoner(rule_vector, &example_io_manager);
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

void test_acyclicity_obvious_cycle() {
    const std::string name = "Acyclicity Obvious Cycle";
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_obvious_cycle_with_time() {
    const std::string name = "Acyclicity Obvious Cycle With Time";
    std::string rule_string = "p(X) := [$, 3][B]r(X)\n"
                              "q(Y) && m(X, Y) := [$, 5][D]p(X)\n"
                              "p(Y) && n(X, Y) := [$, 5][D]q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

void test_acyclicity_only_first_timepoint() {
    // In this test, a cycle will only occur at the first timepoint
    const std::string name = "Acyclicity Only First Timepoint";
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := [$, 3][B]q(X)\n";
    bool expected_has_terminating_chase = false;
    run_acyclicity_test(name, rule_string, expected_has_terminating_chase);
}

//void test_run() {
    //const std::string name = "Test Run";
    //std::string stream_string = "1 4 "
                                //"1 : q(x1, y1, z1)\n"
                                //"2 : q(x1, y1, z1)\n"
                                //"3 : q(x1, y1, z1)\n"
                                //"4 : \n";
    //std::string rule_string =
        //"p(a, b, X, Z) && [I, a] && [I, b] := [$, 100] [D] q(X, Y, Z)\n"
        //" r(X, Z, c, d) && [I, c] && [I, d]  := [$, 100] [D] p(a, b, X, Z)\n";
    //auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    //run(name, stream_string, rule_string, chase_alg);
//}

//void test_run_no_events() {
    //const std::string name = "Test Run";
    //std::string stream_string = "1 4 "
                                //"1 : q(x1, y1, z1)\n"
                                //"2 : q(x1, y1, z1)\n"
                                //"3 : q(x1, y1, z1)\n"
                                //"4 : \n";
    //std::string rule_string =
        //"p(a, b, X, Z) := [$, 100] [D] q(X, Y, Z)\n"
        //" r(X, Z, c, d) := [$, 100] [D] p(a, b, X, Z)\n";
    //auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    //run(name, stream_string, rule_string, chase_alg);
//}

void test_run() {
    const std::string name = "Test Run";
    std::string stream_string = "1 14 "
                                "1 : \n"
                                "2 : c(x2, y2)\n"
                                "2 : d(x2, y2)\n"
                                "4 : \n"
                                "5 : \n"
                                "5 : e(x5, y5), e(a5, b5), e(x5, y5)\n"
                                "5 : e(x5, y5)\n"
                                "6 : a(x6, y6, z6)\n"
                                "7 : \n"
                                "8 : a(x8, y8, z8), b(y8, z8)\n"
                                "9 : \n"
                                "10 : a(x10, y10, z10)\n"
                                "11 : f(a11)\n"
                                "11 : f(-9.099)\n"
                                "11 : f(Z)\n"
                                "12 : e(x12, x12)\n"
                                "13 : \n"
                                "14 : \n";
    std::string rule_string = "q(X, Y, Z) := [D] a(X, Y, Z)\n"
                              "u(X, X) := [D]f(X)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    run(name, stream_string, rule_string, chase_alg);
}

int main() {
    //test_run_no_events();
    test_run();
    // test_acyclicity_simple();
    // test_acyclicity_obvious_cycle();
    // test_acyclicity_obvious_cycle_with_time();
    // test_acyclicity_only_first_timepoint();
    exit(0);
}
