#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

TEST(ExistentialTest, ExistentialSimple) {

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


TEST(ExistentialTest, ExistentialLoop) {

    std::string stream_string = "1 4 "
                                "1 : Bicycle(x1)\n"
                                "2 : Bicycle(x2), Bicycle(x3)\n"
                                "3 : \n"
                                "4 : \n";

    std::string rule_string = "E(v) hasPart(X, v) := Bicycle(X)\n"
                              "Wheel(V) := hasPart(X, V) && Bicycle(X)\n"
                              "E(w) properPartOf(X, w) := Wheel(X)\n"
                              "Bicycle(W) := properPartOf(X, W) && Wheel(X)\n"
                              "partOf(X, Y) := properPartOf(X, Y) \n"
                              "hasPart(X, Y) := partOf(Y, X) \n"
                              "partOf(X, Y) := hasPart(Y, X) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> Bicycle(w0) Wheel(v0) hasPart(w0, v0) hasPart(x1, v0) partOf(v0, w0) partOf(v0, x1) properPartOf(v0, w0)";
    expected[2] = "2 -> Bicycle(w1) Bicycle(w2) Wheel(v1) Wheel(v2) hasPart(w1, v1) hasPart(w2, v2) hasPart(x2, v1) hasPart(x3, v2) partOf(v1, w1) partOf(v1, x2) partOf(v2, w2) partOf(v2, x3) properPartOf(v1, w1) properPartOf(v2, w2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected);
}

TEST(ExistentialTest, ExistentialTimeRefHead) {

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

TEST(ExistentialTest, ExistentialTimeRefBody1) {

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

TEST(ExistentialTest, ExistentialTimeRefBody2) {

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

TEST(ExistentialTest, ExistentialTimeRefHandB) {

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