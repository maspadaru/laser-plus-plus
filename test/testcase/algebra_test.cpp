#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(AlgebraTest, AlgebraAll) {
    std::string stream_string = "1 4 "
                                "1 : p(2), q(1) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "plus(Z) := p(X) && q(Y) && +(Z, X, Y)\n"
                    "minus(Z) := p(X) && q(Y) && -(Z, X, Y)\n"
                    "times(Z) := p(X) && q(Y) && *(Z, X, Y)\n"
                    "div(Z) := p(X) && q(Y) && /(Z, X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> plus(3) minus(1) times(6) div(2)";
    expected[2] = "2 -> ";
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
    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(AlgebraTest, AlgebraMultiple) {
    std::string stream_string = "1 4 "
                                "1 : p(3, 4) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "result(B, A) := -(B, A, X) && +(A, X, Y) && p(X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> result(4)";
    expected[2] = "2 -> ";
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
    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(AlgebraTest, AlgebraDiamond) {
    std::string stream_string = "1 4 "
                                "1 : p(3, 4) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "q(x) := [$, 3][D] p(Y, Z) && +(X, Y, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(7)";
    expected[2] = "2 -> q(7)";
    expected[3] = "3 -> q(7)";
    expected[4] = "4 -> q(7)";
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
