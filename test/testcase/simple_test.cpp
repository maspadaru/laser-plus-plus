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
 

TEST(SimpleTest, SimpleAtom) {

    std::string stream_string = "1 4 "
                                "1 : a(x1, y1, z1)\n"
                                "2 : a(x2, y2, z2)\n"
                                "3 : a(x3, y3, z3)\n"
                                "4 : \n";

    std::string rule_string = "p(X, Y, Z) := a(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> p(x1, y1, z1)";
    expected[1] = "1 -> p(x2, y2, z2)";
    expected[2] = "2 -> p(x3, y3, z3)";
    expected[3] = "3 -> ";
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
        EXPECT_EQ(result, expected[current_time]);
        current_time++;
    }
    // ASSERT_EQ(-1.0, squareRoot(-0.2));
    // EXPECT_TRUE(has_derived_conclusions);
}
