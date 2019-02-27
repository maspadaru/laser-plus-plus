//
// Created by mike on 02/27/18.
//

#include "test_framework.h"

namespace test_framework {

void run_test(std::string const &stream_string, std::string const &rule_string, 
         std::vector<std::string> const &expected) {
    auto simple_io_manager = SimpleIOManager(stream_string);
    auto rule_reader = laser::rule::DefaultRuleReader(rule_string);
    auto reasoner =
        laser::reasoner::SequentialReasoner(&rule_reader, &simple_io_manager);

    reasoner.start();
    auto start_time = simple_io_manager.read_stream_start_time();
    auto end_time = simple_io_manager.read_stream_end_time();
    for (auto current_time = start_time; current_time <= end_time;
         current_time++) {
        std::string result = simple_io_manager.get_output(current_time);
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

} // namespace test_framework 
