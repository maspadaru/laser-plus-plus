//
// Created by mike on 7/17/18.
//

#include "rule/default_rule_reader.h"


namespace laser {
namespace rule {

DefaultRuleReader::DefaultRuleReader(
        std::string rule_string) :
        rule_string(std::move(rule_string)) {

}

inline void DefaultRuleReader::syntax_error(std::string const &error_message) const {
    std::string message = "Line: " + std::to_string(line_counter) +
        " -> Syntax Error: " + error_message;
    const char *exception_message = message.c_str();
    throw util::FormatException(exception_message);

}

void DefaultRuleReader::skip_spaces() {
    input >> std::ws; // eats-up any leadin whitespaces. 
}

char DefaultRuleReader::read_next_char() {
    return input.get();
}

void DefaultRuleReader::read_expected_char(char c) {
    if (read_next_char() != c) {
        std::string error_message = "Expected character = "; 
        error_message.push_back(c);
        syntax_error(error_message);
    }    
}

char DefaultRuleReader::peek_next_char() {
    return input.peek();
}

bool DefaultRuleReader::is_next_char_letter() {
    return std::isalpha(peek_next_char());
}

bool DefaultRuleReader::is_next_char_digit() {
    return std::isdigit(peek_next_char());
}

bool DefaultRuleReader::is_next_char_letter_or_digit() {
    return is_next_char_letter() || is_next_char_digit();
}

bool DefaultRuleReader::is_next_char(char c) {
    return peek_next_char() == c;    
}

void DefaultRuleReader::parse_eoln() {
    skip_spaces();
    if (!input.eof()) {
        syntax_error("Expecting EOL"); 
    }
}

laser::formula::Formula* DefaultRuleReader::parse_head() {
    return null_ptr;
}

laser::formula::Formula* DefaultRuleReader::parse_body() {
    return null_ptr;
}

laser::rule::Rule DefaultRuleReader::parse_rule() {
    skip_spaces();
    auto head = parse_head();   
    skip_spaces();
    read_expected_char(':');
    read_expected_char('=');
    skip_spaces();
    auto body = parse_body();
    parse_eoln();
    // return rule
    auto result = laser::rule::Rule(head, body); 
    return result;
}

std::vector<laser::rule::Rule> DefaultRuleReader::get_rules() {
    std::vector<laser::rule::Rule> result; 
    std::stringstream program_stream;
    program_stream.str(rule_string); 
    std::string line;
    line_counter = 0;
    while (std::getline(program_stream, line, '\n')) {
       line_counter++;
       input.str(std::string()); // clearing input stream - not sure if needed
       input.str(line);
       auto rule = parse_rule();     
       result.push_back(rule);
    }
    return result;
} 

} // namespace rule
} // namespace laser

