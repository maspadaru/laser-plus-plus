//
// Created by mike on 02/27/18.
//

#ifndef LASER_TEST_TEST_FRAMEWORK_H
#define LASER_TEST_TEST_FRAMEWORK_H

#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <core/reasoner.h>
#include <example/example_rule_reader.h>
#include <example/example_io_manager.h>

namespace test_framework {

void run_test(std::string const &stream_string, std::string const &rule_string, 
         std::vector<std::string> const &expected);

} // namespace test_framework

#endif // LASER_TEST_TEST_FRAMEWORK_H

