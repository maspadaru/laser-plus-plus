#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(SkolemChaseTest, SkolemSimple) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";

    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a2, z2, b3, x2, z2)";
    expected[3] = "3 -> p(a4, z3, b5, x3, z3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemSimpleNoInertia) {
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x1, y1, z1)\n"
                                "4 : \n";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a2, z1, b3, x1, z1)";
    expected[3] = "3 -> p(a4, z1, b5, x1, z1)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemInertiaFull) {
    std::string stream_string = "1 8 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x2, y2, z2)\n"
                                "4 : q(x1, y1, z1)\n"
                                "5 : q(x1, y2, z1)\n"
                                "6 : q(x1, y2, z2)\n"
                                "7 : q(x2, y2, z2)\n"
                                "8 : \n";
    std::string rule_string =
        "p(a, Z, b, X, Z) && [I, a] && [I, b] := q(X, Y, Z)\n";
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a0, z1, b1, x1, z1)";
    expected[3] = "3 -> p(a2, z2, b3, x2, z2)";
    expected[4] = "4 -> p(a4, z1, b5, x1, z1)";
    expected[5] = "5 -> p(a4, z1, b5, x1, z1)";
    expected[6] = "6 -> p(a6, z2, b7, x1, z2)";
    expected[7] = "7 -> p(a8, z2, b9, x2, z2)";
    expected[8] = "8 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemInertiaPartial) {
    std::string stream_string = "1 7 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x1, y2, z1)\n"
                                "4 : q(x1, y2, z2)\n"
                                "5 : q(x1, y2, z2)\n"
                                "6 : q(x2, y2, z2)\n"
                                "7 : q(x1, y2, z2)\n"
                                "7 : \n";
    std::string rule_string =
        "p(a, Z, b, X, Z) && [I, a] := q(X, Y, Z)\n";
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a0, z1, b2, x1, z1)";
    expected[3] = "3 -> p(a0, z1, b3, x1, z1)";
    expected[4] = "4 -> p(a4, z2, b5, x1, z2)";
    expected[5] = "5 -> p(a4, z2, b6, x1, z2)";
    expected[6] = "6 -> p(a7, z2, b8, x2, z2)";
    expected[7] = "7 -> p(a9, z2, b10, x1, z2)";
    expected[8] = "8 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemTimeRefHead) {

    std::string stream_string =
        "1 4 "
        "1 : problem(sg1) \n"
        "2 : willOverheat(sg1, 2), problem(sg2)\n"
        "3 : problem(sg3), problem(sg4)\n"
        "3 : willOverheat(sg3, 4), willOverheat(sg3, 3)\n"
        "4 : \n";

    std::string rule_string =
        "[@, TIME] shutdown(SG, alert) := willOverheat(SG, TIME) "
        "&& [$, 100] [D] problem(SG) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> shutdown(sg1, alert0)";
    expected[3] = "3 -> shutdown(sg3, alert1)";
    expected[4] = "4 -> shutdown(sg3, alert1)";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
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
        "hasFlat(b, W, T) := exploded(W) && [$, 100] [D] [@, T] Wheel(W) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1, 1)";
    expected[3] = "3 -> hasFlat(b1, w3, 3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
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
        "hasFlat(b, W, T) := [@, T] exploded(W) && [$, 100] [D] Wheel(W) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1, 2)";
    expected[3] = "3 -> hasFlat(b1, w3, 3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemTimeRefHandB) {

    std::string stream_string = "1 4 "
                                "1 : Wheel(w1) \n"
                                "2 : exploded(w1), Wheel(w2)\n"
                                "3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                "3 : Bicycle(b1)\n"
                                "4 : \n";

    std::string rule_string = "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
                              "Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
                              "[@, T] hasFlat(b, W) := [@, T] exploded(W) && "
                              "[$, 100] [D] Wheel(W) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1)";
    expected[3] = "3 -> Bicycle(b1) Wheel(w3) hasFlat(b1, w3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemConjunctionTwo) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, X) && r(b, Z) := q(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, x1) r(b1, z1)";
    expected[2] = "2 -> p(a2, x2) r(b3, z2)";
    expected[3] = "3 -> p(a4, x3) r(b5, z3)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}

TEST(SkolemChaseTest, SkolemConjunctionThree) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, X) && r(b, Z) && s(a, b) := q(X, Y, Z)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, x1) r(b1, z1) s(a0, b1)";
    expected[2] = "2 -> p(a2, x2) r(b3, z2) s(a2, b3)";
    expected[3] = "3 -> p(a4, x3) r(b5, z3) s(a4, b5)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::SKOLEM);
}
