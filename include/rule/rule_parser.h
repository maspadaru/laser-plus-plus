#ifndef LASER_RULE_RULE_PARSER_H
#define LASER_RULE_RULE_PARSER_H

#include <algorithm>
#include <sstream>

#include "formula/extended/atom.h"
#include "formula/extended/box.h"
#include "formula/extended/conjunction.h"
#include "formula/extended/diamond.h"
#include "formula/extended/time_reference.h"
#include "formula/extended/time_window.h"
#include "formula/extended/tuple_window.h"
#include "rule/rule.h"
#include "util/format_exception.h"

namespace laser::rule {

/**
 *
 */
class RuleParser {
public:

  private:
    std::string rule_string;
    std::stringstream input;
    size_t line_counter = 0;

    // Error handling
    inline void syntax_error(std::string const &error_message) const;

    // Parser helper methods
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
    laser::formula::Formula *parse_head_atom();
    laser::formula::Formula *parse_body();
    laser::formula::Formula *parse_formula();
    laser::formula::Formula *parse_complex_formula();
    laser::formula::Formula *parse_binary_formula();
    laser::formula::Formula *parse_binary_operator();
    laser::formula::Formula *parse_term();
    laser::formula::Formula *parse_atom();
    laser::formula::Formula *parse_predicate_atom();
    laser::formula::Formula *parse_comparison_atom();
    laser::formula::Formula *parse_math_atom();
    laser::formula::Formula *parse_unary_formula();
    laser::formula::Formula *parse_diamond();
    laser::formula::Formula *parse_box();
    laser::formula::Formula *parse_negation();
    laser::formula::Formula *parse_time_reference();
    laser::formula::Formula *parse_time_window();
    laser::formula::Formula *parse_tuple_window();
    std::vector<laser::formula::Formula *> parse_formula_vector();

  public:
    explicit RuleParser(std::string rule_string);
    ~RuleParser() = default;

    /**
    *
    * @return Vector of Rule objects
    * @throw FormatException if the input is not in a format that can be
    * properly parsed.
    * @throw ReadException if any problem occurred while reading the input,
    * e.g.: source file is inaccessible
    */
    std::vector<laser::rule::Rule> get_rules();
};

} // namespace laser::rule 

#endif // LASER_RULE_RULE_PARSER_H
