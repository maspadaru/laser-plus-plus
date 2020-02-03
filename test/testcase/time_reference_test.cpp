#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

TEST(TimeReferenceTest, TimeReferenceBody) {
    std::string stream_string = "1 4 "
                                "1 : b(1), b(2)\n"
                                "2 : b(3)\n"
                                "3 : b(4), b(5)\n"
                                "4 : \n";

    std::string rule_string = "a(T, X) := [@, T] b(X) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> a(1, 1) a(1, 2)";
    expected[2] = "2 -> a(2, 3)";
    expected[3] = "3 -> a(3, 4) a(3, 5)";
    expected[4] = "4 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(TimeReferenceTest, TimeReferenceHead) {

    std::string stream_string = "1 10 "
                                "1 : b(1,x1), b(2, x1)\n"
                                "2 : b(7, x2)\n"
                                "3 : b(6, x3), b(9, x3)\n"
                                "4 : \n";

    std::string rule_string = "[@,T]a(X) := b(T, X) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> a(x1)";
    expected[2] = "2 -> a(x1)";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> a(x3)";
    expected[7] = "7 -> a(x2)";
    expected[8] = "8 -> ";
    expected[9] = "9 -> a(x3)";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(TimeReferenceTest, TimeReferenceHeadAndBody) {

    std::string stream_string = "1 14 "
                                "1 : b(1), b(2)\n"
                                "2 : b(3)\n"
                                "3 : b(4), b(5)\n"
                                "4 : \n";

    std::string rule_string = "[@, T]a(X) := [@, T] b(X) \n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> a(2) a(1)";
    expected[2] = "2 -> a(3)";
    expected[3] = "3 -> a(5) a(4)";
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

//TEST(TimeReferenceTest, TimeReferenceRecursive) {

    //std::string stream_string =
        //"1 10 "
        //"1 : a(1,x1), a(2, x1)\n"
        //"2 : a(7, x2),c(y2) a(3, x2), a(2, x2), a(9, x2)\n"
        //"3 : c(y3)\n"
        //"4 : a(5, x4), c(y4)\n"
        //"6 : c(y6)\n"
        //"7 : \n";

    //std::string rule_string = "[@,T]p(X, Y, T) := b(X) && [@,T]c(Y)\n"
                              //"[@,T]b(X) := a(T, X)\n";

    //std::vector<std::string> expected(15);
    //expected[0] = "0 -> ";
    //expected[1] = "1 -> b(x1)";
    //expected[2] = "2 -> p(x1, y2, 2) p(x2, y2, 2) b(x2) b(x1)";
    //expected[3] = "3 -> p(x2, y3, 3) b(x2)";
    //expected[4] = "4 -> ";
    //expected[5] = "5 -> b(x4)";
    //expected[6] = "6 -> ";
    //expected[7] = "7 -> b(x2)";
    //expected[8] = "8 -> ";
    //expected[9] = "9 -> b(x2)";
    //expected[10] = "10 -> ";
    //expected[11] = "11 -> ";
    //expected[12] = "12 -> ";
    //expected[13] = "13 -> ";
    //expected[14] = "14 -> ";

    //test_framework::run_test(stream_string, rule_string, expected,
                             //laser::util::ChaseAlgorithm::OBLIVIOUS);
//}

TEST(TimeReferenceTest, TimeReferenceBodyPastInput) {
    std::string stream_string = "1 4 "
                                "1 : problem(sg1)\n"
                                "2 : \n"
                                "3 : problem(sg1), problem(sg2)\n"
                                "4 : problem(sg1), query(sg1, 1), query(sg1, 2)\n"
                                "4 : query(sg1, 3), query(sg1, 4), query(sg2, 2)\n"
                                "4 : query(sg2, 3)\n";

    std::string rule_string =
        "alarm(SG, T) := query(SG, T) && [@, T] problem(SG)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> alarm(sg1, 1) alarm(sg1, 3) alarm(sg1, 4) alarm(sg2, 3)";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

TEST(TimeReferenceTest, TimeReferenceBodyPastConclusions) {
    std::string stream_string = "1 4 "
                                "1 : overheat(sg1), problem(sg1)\n"
                                "2 : \n"
                                "3 : problem(sg1), problem(sg2)\n"
                                "3 : overheat(sg1), overheat(sg2) \n"
                                "4 : problem(sg1), query(sg1, 1), query(sg1, 2)\n"
                                "4 : query(sg1, 3), query(sg1, 4), query(sg2, 2)\n"
                                "4 : overheat(sg1), query(sg2, 3)\n";

    std::string rule_string =
        "logevent(SG) := overheat(SG) && problem(SG)\n"
        "alarm(SG, T) := query(SG, T) && [@, T] logevent(SG)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> logevent(sg1)";
    expected[2] = "2 -> ";
    expected[3] = "3 -> logevent(sg1) logevent(sg2)";
    expected[4] = "4 -> alarm(sg1, 1) alarm(sg1, 3) alarm(sg1, 4) alarm(sg2, 3) logevent(sg1)";

    test_framework::run_test(stream_string, rule_string, expected,
                             laser::util::ChaseAlgorithm::OBLIVIOUS);
}

