#include <string>
#include <vector>
#include <sstream>

#include <gtest/gtest.h>
#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "simple_io_manager.h"
#include "simple_parser.h"
#include "simple_reader.h"
#include "simple_writer.h"
 

TEST(BoxTest, BoxAtom) {

    std::string stream_string = "1 14 "
                                "1 : f(1), f(a), c(a, b)\n"
                                "2 : f(a), f(1), c(a, b), f(x2)\n"
                                "3 : f(a), f(1), c(a, b), f(x2)\n"
                                "4 : f(a), f(1), c(b, a), f(x2)\n"
                                "5 : f(1), f(a), c(a,b), c(b, a)\n"
                                "6 : f(1), c(b,a), f(a)\n"
                                "7 : f(a)\n"
                                "8 : f(1), f(a), c(a, b), c(6,7), c(1,2)\n"
                                "9 : f(1), f(a), c(a, b), c(6,7), c(1,2)\n"
                                "10 : f(1), f(a), c(a, b), c(6,7), c(1,2)\n"
                                "11 : f(1), f(a), c(6,7), c(1,2)\n"
                                "12 : f(1), f(a), c(6,7)\n"
                                "13 : f(1), f(a)\n"
                                "14 : f(1), f(a)\n";

    std::string rule_string = "q(X) := [B] f(X)\n"
                              "r(Y,X) := [$, 3][B]c(X, Y)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(a) q(1) r(b, a)";
    expected[2] = "2 -> q(1) q(a) r(b, a)";
    expected[3] = "3 -> q(1) q(a) r(b, a)";
    expected[4] = "4 -> q(1) q(a)";
    expected[5] = "5 -> q(1) q(a)";
    expected[6] = "6 -> q(1) q(a)";
    expected[7] = "7 -> q(a)";
    expected[8] = "8 -> q(a)";
    expected[9] = "9 -> q(a)";
    expected[10] = "10 -> q(a)";
    expected[11] = "11 -> q(a) r(2, 1) r(7, 6)";
    expected[12] = "12 -> q(a) r(7, 6)";
    expected[13] = "13 -> q(a)";
    expected[14] = "14 -> q(a)";

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    int current_time = 1;
    program.set_start_time(current_time);

    while (!program.is_done()) {
        program.evaluate();
        std::string result = simple_io_manager.get_output();
        std::istringstream issr(result);
        std::vector<std::string> results(std::istream_iterator<std::string>{issr},
                                 std::istream_iterator<std::string>());
        std::istringstream isse(expected[current_time]);
        std::vector<std::string> expecteds(std::istream_iterator<std::string>{isse},
                                 std::istream_iterator<std::string>());
        bool has_all_rezults = true;
        for (auto const &rezitem : results) {
            has_all_rezults &= 
                std::find(expecteds.begin(), expecteds.end(), rezitem) 
                    != expecteds.end(); 
        }
        bool is_same_size = results.size() == expecteds.size();
        bool error = !(has_all_rezults && is_same_size);
        // if there is a problem, print both so we can compare
        if (error) {
            std::cout << "Laser rez: " << result << std::endl; 
            std::cout << "Expected : " << expected[current_time] << std::endl; 
            EXPECT_FALSE(error);
        }
        current_time++;
    }
}
