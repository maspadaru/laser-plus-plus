#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(ConditionTest, ConditionEqual) {
    std::string stream_string = "1 4 "
                                "1 : p(99), p(100), p(101)\n"
                                "2 : p(77), p(220)\n"
                                "3 : p(47), p(100), p(300)\n"
                                "4 : \n";
    std::string rule_string = "q(X) := p(X) && ?=(X, C) && =(C,100) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(100)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> q(100)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(ConditionTest, ConditionNotEqual) {
    std::string stream_string = "1 4 "
                                "1 : p(99), p(100), p(101)\n"
                                "2 : p(77), p(220)\n"
                                "3 : p(47), p(100), p(300)\n"
                                "4 : \n";
    std::string rule_string = "q(X) := p(X) && !=(X, C) && =(C,100) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(101) q(99)";
    expected[2] = "2 -> q(220) q(77)";
    expected[3] = "3 -> q(300) q(47)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(ConditionTest, ConditionGreather) {
    std::string stream_string = "1 4 "
                                "1 : p(99), p(100), p(101)\n"
                                "2 : p(77), p(220)\n"
                                "3 : p(47), p(100), p(300)\n"
                                "4 : \n";
    std::string rule_string = "q(X) := p(X) && >(X, C) && =(C,100) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(101)";
    expected[2] = "2 -> q(220)";
    expected[3] = "3 -> q(300)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(ConditionTest, ConditionGreatherEqual) {
    std::string stream_string = "1 4 "
                                "1 : p(99), p(100), p(101)\n"
                                "2 : p(77), p(220)\n"
                                "3 : p(47), p(100), p(300)\n"
                                "4 : \n";
    std::string rule_string = "q(X) := p(X) && >=(X, C) && =(C,100) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(100) q(101)";
    expected[2] = "2 -> q(220)";
    expected[3] = "3 -> q(100) q(300)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(ConditionTest, ConditionLesser) {
    std::string stream_string = "1 4 "
                                "1 : p(99), p(100), p(101)\n"
                                "2 : p(77), p(220)\n"
                                "3 : p(47), p(100), p(300)\n"
                                "4 : \n";
    std::string rule_string = "q(X) := p(X) && <(X, C) && =(C,100) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(99)";
    expected[2] = "2 -> q(77)";
    expected[3] = "3 -> q(47)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(ConditionTest, ConditionLesserEqual) {
    std::string stream_string = "1 4 "
                                "1 : p(99), p(100), p(101)\n"
                                "2 : p(77), p(220)\n"
                                "3 : p(47), p(100), p(300)\n"
                                "4 : \n";
    std::string rule_string = "q(X) := p(X) && <=(X, C) && =(C,100) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(99) q(100)";
    expected[2] = "2 -> q(77)";
    expected[3] = "3 -> q(47) q(100)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}

TEST(ConditionTest, ConditionString) {
    std::string stream_string = "1 4 "
                                "1 : p(monday), p(moon), p(mop)\n"
                                "2 : \n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string =
        "eq(X) := p(X) && ?=(X, MOON) && =(MOON, moon) \n"
        "neq(X) := p(X) && !=(X, MOON) && =(MOON, moon) \n"
        "gt(X) := p(X) && >(X, MOON) && =(MOON, moon) \n"
        "gte(X) := p(X) && >=(X, MOON) && =(MOON, moon) \n"
        "lt(X) := p(X) && <(X, MOON) && =(MOON, moon) \n"
        "lte(X) := p(X) && <=(X, MOON) && =(MOON, moon) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> eq(moon) neq(monday) neq(mop) gt(mop) gte(moon) "
                  "gte(mop) lt(monday) lte(monday) lte(moon)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected, chase_alg);
}
