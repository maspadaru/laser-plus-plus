#include "test_framework.h"

namespace test_framework {

void run_test(std::string const &stream_string, std::string const &rule_string,
              std::vector<std::string> const &expected,
              laser::util::ChaseAlgorithm chase_algorithm) {
    laser::util::Settings::get_instance().set_chase_algorithm(chase_algorithm);
    auto example_io_manager = laser::example::ExampleIOManager(stream_string);
    auto rule_vector = laser::rule::RuleParser(rule_string).get_rules();
    auto reasoner = laser::core::Reasoner(&rule_vector, &example_io_manager);
    reasoner.start();
    auto start_time = example_io_manager.read_stream_start_time();
    auto end_time = example_io_manager.read_stream_end_time();
    for (auto current_time = start_time; current_time <= end_time;
         current_time++) {
        std::string result = example_io_manager.get_output(current_time);
        std::istringstream issr(result);
        std::vector<std::string> results(
            std::istream_iterator<std::string>{issr},
            std::istream_iterator<std::string>());
        std::istringstream isse(expected[current_time]);
        std::vector<std::string> expecteds(
            std::istream_iterator<std::string>{isse},
            std::istream_iterator<std::string>());
        bool has_all_rezults = true;
        for (auto const &rezitem : results) {
            has_all_rezults &= std::find(expecteds.begin(), expecteds.end(),
                                         rezitem) != expecteds.end();
        }
        bool is_same_size = results.size() == expecteds.size();
        bool error = !(has_all_rezults && is_same_size);
        // if there is a problem, print both so we can compare
        if (error) {
            std::cout << "Laser rez: " << result << std::endl;
            std::cout << "Expected : " << expected[current_time] << std::endl;
            EXPECT_FALSE(error);
        }
    }
}

void run_acyclicity_test(std::string const &rule_string, bool expected) {
    bool acyclicity_result;
    laser::util::Settings::get_instance().set_chase_algorithm(
        laser::util::ChaseAlgorithm::SKOLEM);
    auto rule_parser = laser::rule::RuleParser(rule_string);
    auto rule_vector = rule_parser.get_rules();
    auto naive_smfa = laser::acyclicity::NaiveSMFA(rule_vector);
    acyclicity_result = naive_smfa.has_terminating_chase();
    bool error = acyclicity_result != expected;
    if (error) {
        std::cout << "Laser found program has terminating chase: "
                  << acyclicity_result << std::endl;
        std::cout << "Expected program has terminating chase: " << expected
                  << std::endl;
        EXPECT_FALSE(error);
    }
}

} // namespace test_framework
