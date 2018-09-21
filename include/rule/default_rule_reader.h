//
// Created by mike on 7/17/18.
//

#ifndef LASER_RULE_DEFAULT_RULE_READER_H
#define LASER_RULE_DEFAULT_RULE_READER_H

#include <sstream>
#include <algorithm>
#include <stack>

#include "io/rule_reader.h"
#include "formula/extended/atom.h"
#include "util/format_exception.h"

// Helper functions:

namespace laser {
namespace rule {

/**
 *
 */
class DefaultRuleReader : public laser::rule::RuleReader {
private:
    std::string rule_string;
    std::stringstream input;
    size_t line_counter = 0;
// Helper functions:
    inline void syntax_error(std::string const &error_message) const;

// methods
    void skip_spaces();
    char read_next_char(); 
    void read_expected_char(char c); 
    char peek_next_char(); 
    bool is_next_char_letter(); 
    bool is_next_char_digit();
    bool is_next_char(char c); 
    bool is_next_char_letter_or_digit();

    void parse_eoln();
    laser::formula::Formula* parse_head();
    laser::formula::Formula* parse_body();
    laser::rule::Rule parse_rule(); 

public:

// constructors & destructors
    explicit DefaultRuleReader(std::string  rule_string);
    ~DefaultRuleReader() override = default;

// methods
    std::vector<laser::rule::Rule> get_rules() override;
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_DEFAULT_RULE_READER_H
