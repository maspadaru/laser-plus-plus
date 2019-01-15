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
 

TEST(DiamondTest, DiamondAtom) {
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

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> q(x6, y6, z6)";
    expected[7] = "7 -> q(x6, y6, z6)";
    expected[8] = "8 -> q(x6, y6, z6) q(x8, y8, z8)";
    expected[9] = "9 -> q(x6, y6, z6) q(x8, y8, z8)";
    expected[10] = "10 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6)";
    expected[11] = "11 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";
    expected[12] = "12 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";
    expected[13] = "13 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";
    expected[14] = "14 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";

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
