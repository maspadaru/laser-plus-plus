#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

TEST(Acyclicity_NaiveSMFA, Simple) {
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    bool is_acyclical = true;
    test_framework::run_acyclicity_test(rule_string, is_acyclical);
}

TEST(Acyclicity_NaiveSMFA, ObviousCycle) {
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := q(X)\n";
    bool is_acyclical = false;
    test_framework::run_acyclicity_test(rule_string, is_acyclical);
}

TEST(Acyclicity_NaiveSMFA, ObviousCycleWithTime) {
    std::string rule_string = "p(X) := [$, 3][B]r(X)\n"
                              "q(Y) && m(X, Y) := [$, 5][D]p(X)\n"
                              "p(Y) && n(X, Y) := [$, 5][D]q(X)\n";
    bool is_acyclical = false;
    test_framework::run_acyclicity_test(rule_string, is_acyclical);
}

TEST(Acyclicity_NaiveSMFA, OnlyFirstTimePoint) {
    // In this test, a cycle will only occur at the first timepoint
    std::string rule_string = "p(X) := r(X)\n"
                              "q(Y) && m(X, Y) := p(X)\n"
                              "p(Y) && n(X, Y) := [$, 3][B]q(X)\n";
    bool is_acyclical = false;
    test_framework::run_acyclicity_test(rule_string, is_acyclical);
}

