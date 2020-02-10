#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(AlgebraTest, AlgebraSimple) {
    std::string stream_string = "1 4 "
                                "1 : p(2), q(1) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "plus(Z) := p(X) && q(Y) && +(Z, X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> plus(3.000000)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraAll) {
    std::string stream_string = "1 4 "
                                "1 : p(3), q(2) \n"
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
    expected[1] =
        "1 -> plus(5.000000) minus(1.000000) times(6.000000) div(1.500000)";
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
    std::string rule_string =
        "result(B, A) := -(B, A, X) && +(A, X, Y) && p(X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> result(4.000000, 7.000000)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraDiamond) {
    std::string stream_string = "1 4 "
                                "1 : p(3, 4) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "q(X) := [$, 2][D] p(Y, Z) && +(X, Y, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(7.000000)";
    expected[2] = "2 -> q(7.000000)";
    expected[3] = "3 -> q(7.000000)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraDoublePredicate) {
    std::string stream_string = "1 4 "
                                "1 : p(1, 2), p(3, 4) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "q(R) := p(W, X) && p(Y, Z) && +(R, X, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(4.000000) q(6.000000) q(8.000000)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraDoubleTerm) {
    std::string stream_string = "1 4 "
                                "1 : p(1, 2), p(3, 4), p(5, 5) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "q(R) := p(X, Y) && +(R, X, X)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(2.000000) q(6.000000) q(10.000000)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraDoubleVariable) {
    std::string stream_string = "1 4 "
                                "1 : p(1, 2), p(3, 4), p(5, 5) \n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "q(R) := p(X, X) && p(Y, Y) && +(R, X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(10.000000)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraTimeRefHead) {
    std::string stream_string = "1 4 "
                                "1 : p(a1, 1), p(a1, 3)\n"
                                "2 : p(a2, 2), p(a2, 3)\n"
                                "3 : p(a3, 2)\n"
                                "4 : \n";
    std::string rule_string = "[@, T] q(A) := p(A, X) && +(T, X, Y) && =(Y, 1) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> q(a1)";
    expected[3] = "3 -> q(a2) q(a3)";
    expected[4] = "4 -> q(a1) q(a2)";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraReadingTimeVariable) {
    std::string stream_string = "1 4 "
                                "1 : p(11)\n"
                                "2 : p(22)\n"
                                "3 : p(33), p(30)\n"
                                "4 : \n";
    std::string rule_string =
        "q(T, X, U) := [@, T] p(X) && -(U, T, ONE) && =(ONE, 1) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(1, 11, 0)";
    expected[2] = "2 -> p(2, 22, 1)";
    expected[3] = "3 -> p(3, 33, 2) p(3, 30, 2)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraTimeRefNextTime) {
    std::string stream_string = "1 4 "
                                "1 : b(11)\n"
                                "2 : b(22)\n"
                                "3 : b(33), b(30)\n"
                                "4 : \n";
    std::string rule_string =
        "[@, U] a(X) := [@, T] b(X) && +(U, T, Y) && =(Y, 1) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> a(11)";
    expected[3] = "3 -> a(222)";
    expected[4] = "4 -> a(33) a(30)";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraCondition) {
    // This should return 1 -> a(5); 4 -> a(5) a(6)
    // However it does not, since at T=1:
    //      r(5) -> Z = "5", but +(5.0000, 3, 2) => Z = "5.0000"
    //      The string values are compared, and these values do not match
    //  Therefore, Algebra Atoms should not be used for conditions!
    //  The rule can be re-written as:
    //      a(Z) := p(X) && q(Y) && r(Z) && +(A, X, Y) && ?=(A, Z);
    std::string stream_string = "1 4 "
                                "1 : p(2), q(3), r(5) \n"
                                "2 : p(2), q(3), r(6)\n"
                                "3 : p(3), q(3), r(5)\n"
                                "4 : p(2), p(3), q(3), r(5), r(6)\n";
    std::string rule_string = "a(Z) := p(X) && q(Y) && r(Z) && +(Z, X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(AlgebraTest, AlgebraTimeRefBody) {
    // This doesn't work, should be re-written as:
    // a(T, X) := [@, T] b(X) && -(U, X, Y) && =(Y, 1) && ?=(U,T)
    std::string stream_string = "1 4 "
                                "1 : p(11)\n"
                                "2 : p(22)\n"
                                "3 : p(33), p(30)\n"
                                "4 : \n";
    std::string rule_string =
        "q(T, X) := [@, T] p(X) && -(T, X, Y) && =(Y, 1) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

