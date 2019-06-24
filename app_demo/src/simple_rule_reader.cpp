#include "simple_rule_reader.h"

SimpleRuleReader::SimpleRuleReader(std::string const &program_path) {
    std::ifstream program_file(program_path);
    program_string.assign((std::istreambuf_iterator<char>(program_file)),
                 std::istreambuf_iterator<char>());
    parser = new laser::example::ExampleRuleReader(program_string);
}

SimpleRuleReader::~SimpleRuleReader() { delete parser; }

std::vector<laser::rule::Rule> SimpleRuleReader::get_rules() {
    return parser->get_rules();
}
