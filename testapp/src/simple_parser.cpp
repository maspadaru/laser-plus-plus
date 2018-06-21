//
// Created by mike on 6/21/18.
//

#include "simple_parser.h"

std::tuple<int, std::unordered_map<std::string,
        std::vector<laser::formula::Formula * >>>
SimpleParser::parse_data(
        std::vector<std::string>
        raw_data_vector) const {
    return std::tuple<int, std::unordered_map<std::string,
            std::vector<laser::formula::Formula * >>>();
}

std::vector<laser::rule::Rule *>
SimpleParser::parse_rules(std::vector<std::string> raw_rule_vector) const {
    return std::vector<laser::rule::Rule *>();
}
