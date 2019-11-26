#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(TupleWindowTest, SimpleDiamond) {
    std::string stream_string = "0 14 "
                                "1 : f(x1)\n"
                                "2 : a(x2)\n"
                                "3 : f(x3)\n"
                                "5 : f(x5), f(y5)\n"
                                "6 : \n"
                                "7 : f(z), f(z), f(z)\n"
                                "8 : f(z)\n"
                                "9 : \n"
                                "10 : \n"
                                "11 : \n"
                                "12 : \n"
                                "13 : \n"
                                "14 : \n";

    std::string rule_string = "u(X) := [#, 1][D]f(X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> u(x1)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> u(x3)";
    expected[4] = "4 -> u(x3)";
    expected[5] = "5 -> u(x5) u(y5)";
    expected[6] = "6 -> u(y5)";
    expected[7] = "7 -> u(z)";
    expected[8] = "8 -> u(z)";
    expected[9] = "9 -> u(z)";
    expected[10] = "10 -> u(z)";
    expected[11] = "11 -> u(z)";
    expected[12] = "12 -> u(z)";
    expected[13] = "13 -> u(z)";
    expected[14] = "14 -> u(z)";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(TupleWindowTest, SimpleBox) {
    std::string stream_string = "0 14 "
                                "0 : \n"
                                // 3 timepoints where f(x) occurs within the
                                // tuple window
                                "1 : f(x)\n"
                                "2 : f(x)\n"
                                "3 : f(x)\n"
                                // 3 timepoints where f(y) occurs, but after
                                // timepoint 5, f(y) slides out of the window
                                "4 : f(y)\n"
                                "5 : f(x), f(y), f(a), a(x), a(y), a(z), a(t)\n"
                                "6 : f(y)\n"
                                // f(z) occurs more than 3 times within the
                                // tuple window but only for 2 consecutive
                                // timepoints
                                "7 : f(z), f(z), f(z)\n"
                                "8 : f(z)\n"
                                "9 : \n"
                                // f(a) occurs at 3 timepoints within the
                                // tuple window, but the 3 timepoints are not
                                // consecutive
                                "10 : f(a)\n"
                                "11 : f(a)\n"
                                "12 : \n"
                                "13 : f(a)\n"
                                "14 : \n";

    std::string rule_string = "u(X) := [#, 3][B]f(X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> u(x)";
    expected[4] = "4 -> ";
    expected[5] = "5 -> u(x) u(y) u(a)";
    expected[6] = "6 -> ";
    expected[7] = "7 -> u(z)";
    expected[8] = "8 -> u(z)";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(TupleWindowTest, MultipleRules) {
    std::string stream_string = "0 14 "
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

    std::string rule_string = "q(X, Y, Z) := [#, 3] [D] a(X, Y, Z)\n"
                              "r(Y) := [#, 100] c(X, Y)\n"
                              "s(X) := [#, 0] [D] e(X, X)\n"
                              "u(X, X) := [#, 1][D]f(X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> r(y2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> q(x6, y6, z6)";
    expected[7] = "7 -> q(x6, y6, z6)";
    expected[8] = "8 -> q(x6, y6, z6) q(x8, y8, z8)";
    expected[9] = "9 -> q(x6, y6, z6) q(x8, y8, z8)";
    expected[10] = "10 -> q(x10, y10, z10) q(x8, y8, z8)";
    expected[11] = "11 -> u(-9.099, -9.099) u(Z, Z) u(a11, a11)";
    expected[12] = "12 -> s(x12)";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}
