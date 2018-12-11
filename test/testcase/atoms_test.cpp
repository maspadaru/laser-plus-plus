#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "simple_io_manager.h"
#include "simple_parser.h"
#include "simple_reader.h"
#include "simple_writer.h"
 

TEST(AtomsTest, Atom) {

    std::string stream_string = "1 14 "
                                "1 : f(x1)\n"
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
                                "12 : e(x12, x12)\n"
                                "13 : \n"
                                "14 : \n";

    std::string rule_string = "q(X, Y, Z) := a(X, Y, Z)\n"
                              "r(Y) := c(X, Y)\n"
                              "s(X) := e(X, X)\n"
                              "t(Y, X) := d(X, Y)\n"
                              "u(X, X) := f(X)\n"
                              "v(X, Y, X, Y) := e(X, Y)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> u(x1, x1)";
    expected[2] = "2 -> r(y2), t(y2, x2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> v(x5, y5, x5, y5), v(a5, b5, a5, b5)";
    expected[6] = "6 -> q(x6, y6, z6)";
    expected[7] = "7 -> ";
    expected[8] = "8 -> q(x8, y8, z8)";
    expected[9] = "9 -> ";
    expected[10] = "10 -> q(x10, y10, z10)";
    expected[11] = "11 -> u(a11, a11)";
    expected[12] = "12 -> s(x12), v(x12, x12, x12, x12)";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    int current_time = 1;
    program.set_start_time(current_time);

    while (!program.is_done()) {
        program.evaluate();
        std::string result = simple_io_manager.get_output();
        EXPECT_EQ(result, expected[current_time]);
        current_time++;
    }
    // ASSERT_EQ(-1.0, squareRoot(-0.2));
    // EXPECT_TRUE(has_derived_conclusions);
}
