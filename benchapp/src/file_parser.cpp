//
// Created by mike on 6/21/18.
//

#include "file_parser.h"

laser::util::DataAtom FileParser::parse_string(std::string const &input) {
    std::vector<std::string> token_list;
    std::string token;
    std::string predicate;
    std::stringstream iss(input);
    iss >> predicate;
    while (iss >> token) {
        token_list.push_back(token);
    }
    laser::util::DataAtom result(predicate, token_list);
    return result;
}

std::vector<laser::util::DataAtom>
FileParser::parse_data(std::vector<std::string> raw_data_vector) {
    std::vector<laser::util::DataAtom> result;
    for (const auto &raw_string : raw_data_vector) {
        if (raw_string.size() > 0) {
            auto fact = parse_string(raw_string);
            result.push_back(fact);
        }
    }
    return result;
}
