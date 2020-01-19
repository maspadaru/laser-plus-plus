#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(RestrictedChaseTest, Simple) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a2, z2, b3, x2, z2)";
    expected[3] = "3 -> p(a4, z3, b5, x3, z3)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedSimpleNoInertia) {
    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x1, y1, z1)\n"
                                "3 : q(x1, y1, z1)\n"
                                "4 : \n";
    std::string rule_string = "p(a, Z, b, X, Z) := q(X, Y, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, z1, b1, x1, z1)";
    expected[2] = "2 -> p(a2, z1, b3, x1, z1)";
    expected[3] = "3 -> p(a4, z1, b5, x1, z1)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedInertiaPartialMultiRule) {
    std::string stream_string = "1 5 "
                                "1 : r(1, 2, 3, 4)\n"
                                "2 : p(1, 2)\n"
                                "3 : r(1, 2, 3, 4), p(1,2)\n"
                                "4 : p(1, 2)\n"
                                "5 : \n";
    std::string rule_string =
        "q(B, C, D) := r(A, B, C, D)\n"
        "q(B, y, z) && [I, z] := p(A, B)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> q(2, 3, 4)";
    expected[2] = "2 -> q(2, y0, 4)";
    expected[3] = "3 -> q(2, 3, 4)";
    expected[4] = "4 -> q(2, y1, 4)";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedInertiaDatabaseNoise) {
    // Making sure that inertia values are not mixed with values from db
    std::string stream_string = "1 5 "
                                "1 : p(1, 2), q(2)\n"
                                "2 : p(1, 2), q(2), p(4, 5), q(5)\n"
                                "3 : p(4, 5), q(5)\n"
                                "4 : p(1, 2), q(2), p(4, 5), q(5)\n"
                                "5 : \n";
    std::string rule_string =
            "s(z) && t(A, B, z) && u(A, z) && [I, z] := p(A, B) && q(B)\n"
            "t(A, B, B) := p(A, B) && q(B) \n"
            "u(A, B) := p(A, B) && q(B) \n"
            "t(A, B, A) := p(A, B) && q(A) \n"
            "u(A, A) := p(A, B) && q(A) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> s(z0) t(1, 2, 2) t(1, 2, z0) u(1, 2) u(1, z0)";
    expected[2] = "2 -> s(z0) s(z1) t(1, 2, 2) t(1, 2, z0) t(4, 5, 5) t(4, 5, z1) u(1, 2) u(1, z0) u(4, 5) u(4, z1)";
    expected[3] = "3 -> s(z1) t(4, 5, 5) t(4, 5, z1) u(4, 5) u(4, z1)";
    expected[4] = "4 -> s(z1) s(z2) t(1, 2, 2) t(1, 2, z2) t(4, 5, 5) t(4, 5, z1) u(1, 2) u(1, z2) u(4, 5) u(4, z1)";
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedInertiaMultiRuleOne) {
    std::string stream_string = "1 5 "
                                "1 : r(1, 2, 3, 4)\n"
                                "2 : p(1, 2)\n"
                                "3 : r(1, 2, 3, 4), p(1, 2)\n"
                                "4 : p(1,2)\n"
                                "5 : \n";
    std::string rule_string =
        "q(B, C) := r(A, B, C, D)\n"
        "q(B, z) && [I, z] := p(A, B)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> "; 
    expected[1] = "1 -> q(2, 3)"; 
    expected[2] = "2 -> q(2, 3)"; 
    expected[3] = "3 -> q(2, 3) ";  
    expected[4] = "4 -> q(2, 3)"; 
    expected[5] = "5 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedInertiaMultiRuleTwo) {
    std::string stream_string = "1 9 "
                                "1 : r(1, 2, 3, 4)\n"
                                "2 : p(1, 2)\n"
                                "3 : \n"
                                "4 : r(1, 2, 3, 4), p(1, 2)\n"
                                "5 : r(1, 2, 3, 4), p(1, 2)\n"
                                "6 : \n"
                                "7 : r(1, 2, 3, 4)\n"
                                "8 : r(1, 2, 3, 4), p(1, 2)\n"
                                "9 : \n";
    std::string rule_string =
        "q(B, z) && [I, z] := r(A, B, C, D)\n"
        "q(B, z) && [I, z] := p(A, B)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> "; 
    expected[1] = "1 -> q(2, z0)"; 
    expected[2] = "2 -> q(2, z0)";  
    expected[3] = "3 -> ";  
    expected[4] = "4 -> q(2, z1)";  
    expected[5] = "5 -> q(2, z1) ";
    expected[6] = "6 -> ";
    expected[7] = "7 -> q(2, z2)"; 
    expected[8] = "8 -> q(2, z2)";  
    expected[9] = "9 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedInertiaFull) {
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
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
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
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedInertiaPartial) {
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
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
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
                             chase_alg);
}

TEST(RestrictedChaseTest, Loop) {
    std::string stream_string = "1 4 "
                                "1 : Bicycle(x1)\n"
                                "2 : Bicycle(x2), Bicycle(x3)\n"
                                "3 : \n"
                                "4 : \n";
    std::string rule_string = "hasPart(X, v) := Bicycle(X)\n"
                              "Wheel(V) := hasPart(X, V) && Bicycle(X)\n"
                              "properPartOf(X, w) := Wheel(X)\n"
                              "Bicycle(W) := properPartOf(X, W) && Wheel(X)\n"
                              "partOf(X, Y) := properPartOf(X, Y) \n"
                              "hasPart(X, Y) := partOf(Y, X) \n"
                              "partOf(X, Y) := hasPart(Y, X) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> Bicycle(w0) Wheel(v0) hasPart(w0, v0) hasPart(x1, v0) "
                  "partOf(v0, w0) partOf(v0, x1) properPartOf(v0, w0)";
    expected[2] = "2 -> Bicycle(w1) Bicycle(w2) Wheel(v1) Wheel(v2) "
                  "hasPart(w1, v1) hasPart(w2, v2) hasPart(x2, v1) hasPart(x3, "
                  "v2) partOf(v1, w1) partOf(v1, x2) partOf(v2, w2) partOf(v2, "
                  "x3) properPartOf(v1, w1) properPartOf(v2, w2)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

//TEST(RestrictedChaseTest, TimeRefHead) {
    //std::string stream_string =
        //"1 4 "
        //"1 : problem(sg1) \n"
        //"2 : willOverheat(sg1, 2), problem(sg2)\n"
        //"3 : problem(sg3), problem(sg4)\n"
        //"3 : willOverheat(sg3, 4), willOverheat(sg3, 3)\n"
        //"4 : \n";
    //std::string rule_string =
        //"[@, TIME] shutdown(SG, alert) := willOverheat(SG, TIME) "
        //"&& [$, 100] [D] problem(SG) \n";
    //auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    //std::vector<std::string> expected(15);
    //expected[0] = "0 -> ";
    //expected[1] = "1 -> ";
    //expected[2] = "2 -> shutdown(sg1, alert0)";
    //expected[3] = "3 -> shutdown(sg3, alert1)";
    //expected[4] = "4 -> shutdown(sg3, alert2)";
    //test_framework::run_test(stream_string, rule_string, expected,
                             //chase_alg);
//}

TEST(RestrictedChaseTest, TimeRefBody1) {
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
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1, 1)";
    expected[3] = "3 -> hasFlat(b1, w3, 3)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, TimeRefBody2) {

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
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> hasFlat(b0, w1, 2)";
    expected[3] = "3 -> hasFlat(b1, w3, 3)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

//TEST(RestrictedChaseTest, TimeRefHandB) {
    //std::string stream_string = "1 4 "
                                //"1 : Wheel(w1) \n"
                                //"2 : exploded(w1), Wheel(w2)\n"
                                //"3 : Wheel(w3), Wheel(w4), exploded(w3)\n"
                                //"3 : Bicycle(b1)\n"
                                //"4 : \n";
    //std::string rule_string = "Wheel(W) := hasFlat(B, W) && Bicycle(B)\n"
                              //"Bicycle(B) := hasFlat(B, W) && Wheel(W)\n"
                              //"[@, T] hasFlat(b, W) := [@, T] exploded(W) && "
                              //"[$, 100] [D] Wheel(W) \n";
    //auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    //std::vector<std::string> expected(15);
    //expected[0] = "0 -> ";
    //expected[1] = "1 -> ";
    //expected[2] = "2 -> hasFlat(b0, w1)";
    //expected[3] = "3 -> Bicycle(b1) Wheel(w3) hasFlat(b1, w3)";
    //expected[4] = "4 -> ";
    //test_framework::run_test(stream_string, rule_string, expected,
                             //chase_alg);
//}

TEST(RestrictedChaseTest, ConjunctionTwo) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, X) && r(b, Z) := q(X, Y, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, x1) r(b1, z1)";
    expected[2] = "2 -> p(a2, x2) r(b3, z2)";
    expected[3] = "3 -> p(a4, x3) r(b5, z3)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, ConjunctionThree) {

    std::string stream_string = "1 4 "
                                "1 : q(x1, y1, z1)\n"
                                "2 : q(x2, y2, z2)\n"
                                "3 : q(x3, y3, z3)\n"
                                "4 : \n";
    std::string rule_string = "p(a, X) && r(b, Z) && s(a, b) := q(X, Y, Z)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(a0, x1) r(b1, z1) s(a0, b1)";
    expected[2] = "2 -> p(a2, x2) r(b3, z2) s(a2, b3)";
    expected[3] = "3 -> p(a4, x3) r(b5, z3) s(a4, b5)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedSimple) {
    // At the same timepoint
    std::string stream_string = "1 4 "
                                "1 : q(x1)\n"
                                "2 : s(x2, y2)\n"
                                "3 : q(x3), s(x3, y3)\n"
                                "4 : \n";
    std::string rule_string = "p(X, z) := q(X)\n"
                              "p(X, Y) := s(X, Y) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(x1, z0)";
    expected[2] = "2 -> p(x2, y2)";
    expected[3] = "3 -> p(x3, y3)";
    expected[4] = "4 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedConjunctionBody) {
    // A conjunction is pressent is the body of the existential rule
    std::string stream_string = "1 4 "
                                "1 : q(x1)\n"
                                "2 : s(x2, y2)\n"
                                "3 : q(x3), s(x3, y3)\n"
                                "4 : q(x4), s(x4, y4), t(x4, y4, z4)\n";
    std::string rule_string = "r(X, Y, z)  := p(X, Y) && q(X)\n"
                              "p (X, Y) := s(X, Y) \n"
                              "r(X, Y, Z) := t(X, Y, Z)";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> p(x2, y2)";
    expected[3] = "3 -> p(x3, y3) r(x3, y3, z0)";
    expected[4] = "4 -> p(x4, y4) r(x4, y4, z4)";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedConjunctionHeadPaper) {
    // Example from paper: "Efficient Model Construction for Horn Logic
    // with VLog - System Description" - J. Urbani, M. Krotzsch, I. Dragoste,
    // David Carral - 2018
    // In this example, the computation will terminate after evaluating each
    // rule once. When atemting to re-evaluate the first rule on conclusion:
    // Bicycle(w0), the restricted chase finds that:
    // hasPart(w0, v0) && Wheel(v0)
    // are in the database, so new null values such as
    // hasPart(w0, v1) && Wheel(v1)
    // are not generated. Thus, the computation terminates due to restricted
    // chase conditions.
    // See paper for details.
    std::string stream_string = "1 2 "
                                "1 : Bicycle(c) \n"
                                "2 : \n";
    std::string rule_string = "hasPart(X, v) && Wheel(v) := Bicycle(X)\n"
                              "properPartOf(X, w) && Bicycle(w) := Wheel(X)\n"
                              "partOf(X, Y) := properPartOf(X, Y)\n"
                              "partOf(Y, X) := hasPart(X, Y)\n"
                              "hasPart(Y, X) := partOf(X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> hasPart(c, v0) Wheel(v0) properPartOf(v0, w0) "
                  "Bicycle(w0) partOf(v0, w0) partOf(v0, c) hasPart(w0, v0)";
    expected[2] = "2 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedConjunctionHeadSwap) {
    // see ExistentialRestrictedConjunctionHeadPaper
    // same exaple, but atoms in head are swaped in the first rule
    std::string stream_string = "1 2 "
                                "1 : Bicycle(c) \n"
                                "2 : \n";
    std::string rule_string = "Wheel(v) && hasPart(X, v) := Bicycle(X)\n"
                              "properPartOf(X, w) && Bicycle(w) := Wheel(X)\n"
                              "partOf(X, Y) := properPartOf(X, Y)\n"
                              "partOf(Y, X) := hasPart(X, Y)\n"
                              "hasPart(Y, X) := partOf(X, Y)\n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> hasPart(c, v0) Wheel(v0) properPartOf(v0, w0) "
                  "Bicycle(w0) partOf(v0, w0) partOf(v0, c) hasPart(w0, v0)";
    expected[2] = "2 -> ";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RestrictedChaseTest, RestrictedWindow) {
    // At diferent timepoints
    std::string stream_string = "1 4 "
                                "1 : s(x1, y1),q(x1)\n"
                                "2 : q(x2)\n"
                                "3 : q(x1)\n"
                                "4 : q(x4)\n";
    std::string rule_string = "p(X, z)  := q(X)\n"
                              "p (X, Y) := [$, 2] [D] s(X, Y) \n";
    auto chase_alg = laser::util::ChaseAlgorithm::RESTRICTED;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> p(x1, y1)";
    expected[2] = "2 -> p(x1, y1) p(x2, z0)";
    expected[3] = "3 -> p(x1, y1)";
    expected[4] = "4 -> p(x4, z1)";
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}
