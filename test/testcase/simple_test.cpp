#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

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

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}
