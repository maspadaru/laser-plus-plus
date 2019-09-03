#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(RecursiveTest, RecursiveSimple) {
    std::string stream_string = "1 6 "
                                "1 : p(1), q(1)\n"
                                "2 : p(1)\n"
                                "3 : p(1), q(1)\n"
                                "4 : \n";
    std::string rule_string = "b(X) := [$, 3] [D] d(X)\n"
                              "d(X) := q(X) && p(X)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> b(1) d(1)";
    expected[2] = "2 -> b(1)";
    expected[3] = "3 -> b(1) d(1)";
    expected[4] = "4 -> b(1)";
    expected[5] = "5 -> b(1)";
    expected[6] = "6 -> b(1)";
    expected[7] = "7 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RecursiveTest, RecursiveComplex) {
    std::string stream_string = "1 14 "
                                "1 : q(1), p(1)\n"
                                "2 : p(1)\n"
                                "3 : p(1), q(1)\n"
                                "4 : \n";
    std::string rule_string = "a(X) := b(X) && c(X)\n"
                              "b(X) := [$, 3] [D] d(X)\n"
                              "c(X) := [$, 3] [B] e(X)\n"
                              "e(X) := b(X) && p(X)\n"
                              "d(X) := q(X) && p(X)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> a(1) b(1) c(1) e(1) d(1)";
    expected[2] = "2 -> a(1) b(1) c(1) e(1)";
    expected[3] = "3 -> a(1) b(1) c(1) e(1) d(1)";
    expected[4] = "4 -> b(1)";
    expected[5] = "5 -> b(1)";
    expected[6] = "6 -> b(1)";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}
