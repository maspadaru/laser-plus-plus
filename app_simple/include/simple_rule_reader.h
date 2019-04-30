#ifndef SIMPLEAPP_SIMPLE_RULE_READER_H
#define SIMPLEAPP_SIMPLE_RULE_READER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

#include <rule/rule.h>
#include <rule/rule_reader.h>
#include <example/example_rule_reader.h>

class SimpleRuleReader : public laser::rule::RuleReader {
private:
    std::string program_string;
    RuleReader* parser;
public:
    explicit SimpleRuleReader(std::string const &stream_path);
    ~SimpleRuleReader() override;
    std::vector<laser::rule::Rule> get_rules() override;
};

#endif // SIMPLEAPP_SIMPLE_RULE_READER_H
