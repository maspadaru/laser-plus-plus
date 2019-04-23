#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

TEST(SkolemChaseTest, SkolemSimple) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";

    std::string rule_string = "E(a, b) p(a, Z, b, X, Z) := q(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a2, z2, b3, x2, z2)";
    expected[3] = "3 -> p(a4, z3, b5, x3, z3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(SkolemChaseTest, SkolemTimeRefHead) {

    std::string stream_string = "1 4 "
                                "1 : problem(sg1) \n"
                                "2 : willOverheat(sg1, 2), problem(sg2)\n"
                                "3 : problem(sg3), problem(sg4)\n"
                                "3 : willOverheat(sg3, 4), willOverheat(sg3, 3)\n"
                                "4 : \n";

    std::string rule_string = 
        "E(alert)[@, TIME] shutdown(SG, alert) := willOverheat(SG, TIME) "
                    "&& [$, 100] [D] problem(SG) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> shutdown(sg1, alert0)";
    expected[3] = "3 -> shutdown(sg3, alert2)";
    expected[4] = "4 -> shutdown(sg3, alert1)";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(SkolemChaseTest, SkolemTimeRefBody1) {

    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";

    std::string rule_string = 
        "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
        "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
        "E(b) hasFlat(b, W, T) := exploded(W) && [$, 100] [D] [@, T] Wheel(W) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1, 1)";
    expected[3] = "3 -> hasFlat(b1, w3, 3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(SkolemChaseTest, SkolemTimeRefBody2) {

    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";

    std::string rule_string = 
        "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
        "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
        "E(b) hasFlat(b, W, T) := [@, T] exploded(W) && [$, 100] [D] Wheel(W) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1, 2)";
    expected[3] = "3 -> hasFlat(b1, w3, 3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(SkolemChaseTest, SkolemTimeRefHandB) {

    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";

    std::string rule_string = 
        "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
        "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
        "E(b) [@, T] hasFlat(b, W) := [@, T] exploded(W) && [$, 100] [D] Wheel(W) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1)";
    expected[3] = "3 -> Bicycle(b1) Wheel(w3) hasFlat(b1, w3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(SkolemChaseTest, SkolemConjunctionTwo) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "E(a, b) p(a, X) && r(b, Z) := q(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, x1) r(b1, z1)";
    expected[2] = "2 -> p(a2, x2) r(b3, z2)";
    expected[3] = "3 -> p(a4, x3) r(b5, z3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(SkolemChaseTest, SkolemConjunctionThree) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "E(a, b) p(a, X) && r(b, Z) && s(a, b) := q(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, x1) r(b1, z1) s(a0, b1)";
    expected[2] = "2 -> p(a2, x2) r(b3, z2) s(a2, b3)";
    expected[3] = "3 -> p(a4, x3) r(b5, z3) s(a4, b5)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

