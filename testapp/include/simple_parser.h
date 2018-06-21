//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_PARSER_H
#define TEST_SIMPLE_PARSER_H


#include <tuple>
#include <vector>

#include <input/data_parser.h>
#include <input/rule_parser.h>
#include <rule/rule.h>
#include <formula/formula.h>

class SimpleParser
        : public laser::input::DataParser, public laser::input::RuleParser {
private:

public:
    ~SimpleParser() override = default;

    std::tuple<int, std::unordered_map<std::string, std::vector<laser::formula::Formula *>>>
    parse_data(std::vector<std::string> raw_data_vector) const override;

    std::vector<laser::rule::Rule *>
    parse_rules(std::vector<std::string> raw_rule_vector) const override;

};


#endif // TEST_SIMPLE_PARSER_H
