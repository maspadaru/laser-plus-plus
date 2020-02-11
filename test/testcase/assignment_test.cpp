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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
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
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AssignmentTest, AssignmentCondition) {
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1) \n"
                                "2 : q(x0, y2) q(x2, y2) \n"
                                "3 : \n"
                                "4 : q(x0, y4), q(x0, y3), q(x0, y4)\n";
    std::string rule_string = "p(X, Y) := q(X, Y) && =(X, x0)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> p(x0, y2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> p(x0, y4) p(x0, y3)";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AssignmentTest, AassignmentTimeReference) {
    std::string stream_string = "1 4 "
                                "1 : b(1)\n"
                                "2 : b(2)\n"
                                "3 : b(33), b(30)\n"
                                "4 : b(4)\n";
    std::string rule_string =
        "[@, T] a(X) := b(X) && =(T, 4) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> a(1) a(2) a(4) a(33) a(30)";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}
