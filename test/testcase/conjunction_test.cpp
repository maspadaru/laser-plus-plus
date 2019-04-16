#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

TEST(ConjunctionTest, SameVariables) {
    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(x)\n"
                                "3 : a(x)\n"
                                "4 : a(x)\n"
                                "5 : a(x), b(x)\n"
                                "6 : b(x)\n"
                                "7 : b(x)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(x)\n"
                                "11 : a(x)\n"
                                "12 : a(x)\n"
                                "13 : a(x)\n"
                                "14 : a(x)\n";

    std::string rule_string = "q(X) := a(X) &&  b(X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> q(x)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> q(x)";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> q(x)";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(ConjunctionTest, TwoVariables) {

    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(y)\n"
                                "3 : a(x)\n"
                                "4 : a(x)\n"
                                "5 : a(x), b(y)\n"
                                "6 : b(x)\n"
                                "7 : b(x)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(y)\n"
                                "11 : a(x)\n"
                                "12 : a(x)\n"
                                "13 : a(x)\n"
                                "14 : a(x)\n";

    std::string rule_string = "q(X,Y) := a(X) &&  b(Y)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> q(x, y)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> q(x, y)";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> q(x, y)";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(ConjunctionTest, ConjunctionDiamond) {

    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(y)\n"
                                "3 : a(x)\n"
                                "4 : a(x)\n"
                                "5 : a(x), b(y)\n"
                                "6 : b(y)\n"
                                "7 : b(y)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(y)\n"
                                "11 : a(x)\n"
                                "12 : a(x)\n"
                                "13 : a(x)\n"
                                "14 : a(x)\n";

    std::string rule_string = "q(X,Y) := a(X) && [$, 2] [D] b(Y)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> q(x, y)";
    expected[3] = "3 -> q(x, y)";
    expected[4] = "4 -> q(x, y)";
    expected[5] = "5 -> q(x, y)";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> q(x, y)";
    expected[10] = "10 -> q(x, y)";
    expected[11] = "11 -> q(x, y)";
    expected[12] = "12 -> q(x, y)";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(ConjunctionTest, ConjunctionBox) {
    std::string stream_string = "1 14 "
                                "1 : a(x)\n"
                                "2 : a(x), b(y)\n"
                                "3 : a(x), b(y)\n"
                                "4 : a(x), b(y)\n"
                                "5 : a(x), b(y)\n"
                                "6 : b(y)\n"
                                "7 : b(y)\n"
                                "8 : \n"
                                "9 : a(x)\n"
                                "10 : a(x), b(y)\n"
                                "11 : a(x), b(y)\n"
                                "12 : a(x), b(y)\n"
                                "13 : a(x), b(y)\n"
                                "14 : a(x)\n";

    std::string rule_string = "q(X,Y) := a(X) && [$, 2] [B] b(Y)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> q(x, y)";
    expected[5] = "5 -> q(x, y)";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> q(x, y)";
    expected[13] = "13 -> q(x, y)";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(ConjunctionTest, ConjunctionCrossVariables) {
    std::string stream_string = "1 14 "
                                "1 : a(x,y)\n"
                                "2 : a(x,x), b(y,x)\n"
                                "3 : a(y,y), b(y,x)\n"
                                "4 : a(x,y), b(y,x)\n"
                                "5 : a(y,x), b(y,x)\n"
                                "6 : a(x,x), b(x,y)\n"
                                "7 : a(x,x), b(y,y)\n"
                                "8 : a(x,x), b(y,x)\n"
                                "9 : b(x,y)\n"
                                "10 : \n";

    std::string rule_string = "q(X,Y) := a(X,X) && b(Y,X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> q(x, y)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> q(x, y)";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(ConjunctionTest, ConjunctionSNE) {
    std::string stream_string = "1 4 "
                                "1 : p(x1)\n"
                                "2 : t(x1)\n"
                                "3 : s(x1)\n"
                                "4 : \n";
    std::string rule_string = 
                        "q(X) := t(X) && [$, 3] [D] p(X)\n" // old input fact
                        "r(X) := s(X) && [$, 2] [D] q(X)\n"; // old conclusion

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> q(x1)";
    expected[3] = "3 -> r(x1)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}
