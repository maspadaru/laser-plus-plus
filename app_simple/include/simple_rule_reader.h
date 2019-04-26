//
// Created by mike on 03/07/18.
//

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
#include <formula/extended/atom.h>
#include <formula/extended/diamond.h>
#include <formula/extended/box.h>
#include <formula/extended/time_window.h>
#include <formula/extended/tuple_window.h>
#include <formula/extended/conjunction.h>
#include <formula/extended/time_reference.h>
#include <util/format_exception.h>
#include <util/read_exception.h>

// Helper functions:

/**
 *
 */
class SimpleRuleReader : public laser::rule::RuleReader {
private:
    std::stringstream input;
    std::ifstream program_stream;
    size_t line_counter = 0;

    // Error handling 
    inline void syntax_error(std::string const &error_message) const;

    // Parser helper methods
    bool read_line(std::string &line);
    void skip_spaces();
    void skip_next_char(); 
    char read_next_char(); 
    void skip_expected_char(char c); 
    void skip_conjunction_operator();
    char peek_next_char(); 
    bool is_next_char_letter(); 
    bool is_next_char_digit();
    bool is_next_char(char c); 
    bool is_next_char_letter_or_digit();
    bool is_next_char_binary_operator();
    bool is_next_char_conjunction_operator();
    bool is_next_char_math_operator();
    bool is_next_char_comparison_operator();

    // EBNF Parser Methods

    // characters and identifiers
    void parse_eoln();
    std::string parse_math_sign();
    std::string parse_comparison_sign();
    char parse_nonzero();
    std::string parse_zero();
    char parse_diggit();
    char parse_letter();
    std::string parse_positive_number();
    std::string parse_natural_number();
    std::string parse_integer();
    std::string parse_float();
    std::string parse_identifier();
    std::string parse_argument();
    std::string parse_float_argument();

    // rules and formulas
    std::vector<laser::rule::Rule> parse_program();
    laser::rule::Rule parse_rule(); 
    std::vector<laser::formula::Formula *> parse_head();
    laser::formula::Formula* parse_head_atom();
    laser::formula::Formula* parse_body();
    laser::formula::Formula* parse_formula();
    laser::formula::Formula* parse_complex_formula();
    laser::formula::Formula* parse_binary_formula();
    laser::formula::Formula* parse_binary_operator();
    laser::formula::Formula* parse_term();
    laser::formula::Formula* parse_atom();
    laser::formula::Formula* parse_predicate_atom();
    laser::formula::Formula* parse_comparison_atom();
    laser::formula::Formula* parse_math_atom();
    laser::formula::Formula* parse_unary_formula();
    laser::formula::Formula* parse_diamond();
    laser::formula::Formula* parse_box();
    laser::formula::Formula* parse_negation();
    laser::formula::Formula* parse_time_reference();
    laser::formula::Formula* parse_time_window();
    laser::formula::Formula* parse_tuple_window();
    std::vector<laser::formula::Formula *> parse_formula_vector(); 

public:

// constructors & destructors
    explicit SimpleRuleReader(std::string const &stream_path);
    ~SimpleRuleReader() override;

// methods
    std::vector<laser::rule::Rule> get_rules() override;
};

#endif // SIMPLEAPP_SIMPLE_RULE_READER_H
