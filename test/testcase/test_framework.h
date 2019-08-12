#ifndef LASER_TEST_TEST_FRAMEWORK_H
#define LASER_TEST_TEST_FRAMEWORK_H

#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <core/reasoner.h>
#include <util/chase_algorithm.h>
#include <rule/rule_parser.h>
#include <example/example_io_manager.h>
#include <acyclicity/naive_smfa.h>

namespace test_framework {

void run_test(std::string const &stream_string, std::string const &rule_string, 
         std::vector<std::string> const &expected,
              laser::util::ChaseAlgorithm chase_algorithm);

void run_acyclicity_test(std::string const &rule_string, bool expected);

} // namespace test_framework

#endif // LASER_TEST_TEST_FRAMEWORK_H

