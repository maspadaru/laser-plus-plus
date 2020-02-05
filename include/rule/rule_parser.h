#ifndef LASER_RULE_RULE_PARSER_H
#define LASER_RULE_RULE_PARSER_H

#include <algorithm>
#include <sstream>
#include <set>

#include "formula/extended/atom.h"
#include "formula/extended/box.h"
#include "formula/extended/conjunction.h"
#include "formula/extended/diamond.h"
#include "formula/extended/time_reference.h"
#include "formula/extended/time_window.h"
#include "formula/extended/tuple_window.h"
#include "formula/math/assignment.h"
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
    std::set<std::string> inertia_variables;

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
    bool is_next_char_algebra_operator();
    bool is_next_char_condition_operator();
    bool is_next_char_equals_sign();

    // EBNF Parser Methods

    // characters and identifiers
    std::string parse_algebra_operator();
    std::string parse_condition_operator();
    std::string parse_assignment_operator();
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
    std::vector<std::string> parse_argument_vector(); 

    // rules and formulas
    std::unique_ptr<laser::formula::Formula> parse_assignment_atom();
    std::unique_ptr<laser::formula::Formula> parse_condition_atom();
    std::unique_ptr<laser::formula::Formula> parse_algebra_atom();
    std::vector<std::unique_ptr<laser::rule::Rule>> parse_program();
    std::unique_ptr<laser::rule::Rule> parse_rule();
    std::vector<std::unique_ptr<laser::formula::Formula>> parse_head();
    std::unique_ptr<laser::formula::Formula> parse_head_atom();
    std::unique_ptr<laser::formula::Formula> parse_body();
    std::unique_ptr<laser::formula::Formula> parse_complex_formula();
    std::unique_ptr<laser::formula::Formula> parse_binary_formula();
    std::unique_ptr<laser::formula::Formula> parse_binary_operator();
    std::unique_ptr<laser::formula::Formula> parse_term();
    std::unique_ptr<laser::formula::Formula> parse_atom();
    std::unique_ptr<laser::formula::Formula> parse_predicate_atom();
    std::unique_ptr<laser::formula::Formula> parse_comparison_atom();
    std::unique_ptr<laser::formula::Formula> parse_math_atom();
    std::unique_ptr<laser::formula::Formula> parse_unary_formula();
    std::unique_ptr<laser::formula::Formula> parse_diamond();
    std::unique_ptr<laser::formula::Formula> parse_box();
    std::unique_ptr<laser::formula::Formula> parse_negation();
    std::unique_ptr<laser::formula::Formula> parse_time_reference();
    std::unique_ptr<laser::formula::Formula> parse_time_window();
    std::unique_ptr<laser::formula::Formula> parse_tuple_window();
    std::unique_ptr<laser::formula::Formula> parse_inertia_operator(); 
    std::unique_ptr<laser::formula::Formula> parse_extended_head_atom(); 
    std::vector<std::unique_ptr<laser::formula::Formula>> parse_formula_vector();

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
    std::vector<std::unique_ptr<laser::rule::Rule>> get_rules();
};

} // namespace laser::rule 

#endif // LASER_RULE_RULE_PARSER_H
