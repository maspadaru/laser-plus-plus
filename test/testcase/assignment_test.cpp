#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(AssignmentTest, AssignmentSimple) {
    std::string stream_string = "1 4 "
                                "1 : \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "p(X) := =(X, 3)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(3)";
    expected[2] = "2 -> p(3)";
    expected[3] = "3 -> p(3)";
    expected[4] = "4 -> p(3)";
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

TEST(AssignmentTest, AssignmentConjunctionLast) {
    std::string stream_string = "1 4 "
                                "1 : q(1) \n"
                                "2 : q(2)\n"
                                "3 : \n"
                                "4 : q(4)\n";
    std::string rule_string = "p(X, Y) := q(Y) && =(X, v)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(v, 1)";
    expected[2] = "2 -> p(v, 2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> p(v, 4)";
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

TEST(AssignmentTest, AssignmentConjunctionFirst) {
    std::string stream_string = "1 4 "
                                "1 : q(1) \n"
                                "2 : q(2)\n"
                                "3 : \n"
                                "4 : q(4)\n";
    std::string rule_string = "p(X, Y) := =(X, v) && q(Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(v, 1)";
    expected[2] = "2 -> p(v, 2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> p(v, 4)";
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


TEST(AssignmentTest, AssignmentConjunctionMiddle) {
    std::string stream_string = "1 4 "
                                "1 : q(1), r(a) \n"
                                "2 : q(2), r(b)\n"
                                "3 : \n"
                                "4 : q(4), r(d)\n";
    std::string rule_string = "p(X, Y, Z) := q(Y) && =(X, v) && r(Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(v, 1, a)";
    expected[2] = "2 -> p(v, 2, b)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> p(v, 4, d)";
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

TEST(AssignmentTest, AssignmentConjunctionConflict) {
    std::string stream_string = "1 4 "
                                "1 : q(1), r(1) \n"
                                "2 : q(2), r(1)\n"
                                "3 : q(1), r(2)\n"
                                "4 : q(1), r(1)\n";
    std::string rule_string = "p(X, X, X) := q(X) && =(X, 1) && r(X)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(1, 1, 1)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> p(1, 1, 1)";
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

TEST(AssignmentTest, AssignmentDiamond) {
    std::string stream_string = "1 4 "
                                "1 : \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "p(X) := [$, 3][D] =(X, v)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(v)";
    expected[2] = "2 -> p(v)";
    expected[3] = "3 -> p(v)";
    expected[4] = "4 -> p(v)";
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

TEST(AssignmentTest, AssignmentBox) {
    std::string stream_string = "1 4 "
                                "1 : \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "p(X) := [$, 3][B] =(X, v)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(v)";
    expected[2] = "2 -> p(v)";
    expected[3] = "3 -> p(v)";
    expected[4] = "4 -> p(v)";
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
