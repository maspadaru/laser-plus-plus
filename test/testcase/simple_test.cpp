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
 

TEST(SimpleTest, SimpleAtom) {

    std::string stream_string = "1 4 "
                                "1 : a(x1, y1, z1)\n"
                                "2 : a(x2, y2, z2)\n"
                                "3 : a(x3, y3, z3)\n"
                                "4 : \n";

    std::string rule_string = "p(X, Y, Z) := a(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(x1, y1, z1)";
    expected[2] = "2 -> p(x2, y2, z2)";
    expected[3] = "3 -> p(x3, y3, z3)";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

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
