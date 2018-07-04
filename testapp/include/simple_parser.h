//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_PARSER_H
#define TEST_SIMPLE_PARSER_H


#include <tuple>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>
#include <utility>

#include <input/data_parser.h>
#include <input/rule_parser.h>
#include <rule/rule.h>
#include <formula/formula.h>
#include <excetion/format_exception.h>
#include <formula/extended/atom.h>

enum class TokenType {
    OPERATOR,
    OPEN_PARENTHESES,
    CLOSED_PARENTHESES,
    ENTAILMENT_SIGN,
    IDENTIFIER
};

struct Token {
    TokenType type;
    std::string value;
};

struct PseudoFormula {
    PseudoFormula() = default;
    PseudoFormula(
            laser::formula::FormulaType type, std::string predicate,
            std::vector<std::string> arguments);

    // TODO needs to be extended for the other Formula types
    laser::formula::FormulaType type;
    std::string predicate;
    std::vector<std::string> arguments;

};

// Helper functions:

static inline void rtrim(std::string *s);

static inline void ltrim(std::string *s);

static inline void trim(std::string *s);

static inline void syntax_error(std::string error_message);

static inline void push_to_argument_stack(
        laser::formula::Formula *formula,
        std::stack<std::vector<laser::formula::Formula *>> *argument_stack);


class SimpleParser
        : public laser::input::DataParser, public laser::input::RuleParser {
private:

    std::vector<std::vector<PseudoFormula>> argument_stack;

    Token recognize(std::string token_string) const;

    std::vector<Token> tokenize(std::string rule_string) const;

    std::vector<Token> add_token_attempt(
            std::vector<Token> token_vector,
            std::ostringstream &token_stream
    ) const;

    std::vector<Token> add_new_token(
            std::vector<Token> token_vector,
            TokenType type, char value_char) const;

    laser::rule::Rule build_rule(
            PseudoFormula pseudo_head,
            std::vector<PseudoFormula> pseudo_body);

    bool is_unary_operator(Token token) const;

    bool is_binary_operator(Token token) const;

    void parse_operator(Token token, bool is_head = false);

    std::tuple<size_t, std::vector<std::string>> parse_predicate_arguments(
            size_t index, std::vector<Token> *tokens) const;

    PseudoFormula argument_stack_pop();

    std::vector<PseudoFormula> argument_stack_pop_vector();

    void argument_stack_push(PseudoFormula formula);

    void argument_stack_push_vector(
            std::vector<PseudoFormula> formula_vector);

    std::tuple<PseudoFormula, std::vector<PseudoFormula>>
    parse_token_vector(std::vector<Token> token_vector);

    void append_new_data(
            std::vector<PseudoFormula> pseudo_formulas,
            std::unordered_map<std::string, std::vector<laser::formula::Formula *>>
            *formula_map);

    laser::formula::Formula *find_or_add_formula(
            PseudoFormula pseudo_formula,
            std::vector<laser::formula::Formula *> *formula_vector);

public:
    ~SimpleParser() override = default;

    std::tuple<size_t, std::unordered_map<std::string, std::vector<laser::formula::Formula *>>>
    parse_data(std::vector<std::string> raw_data_vector) override;

    std::vector<laser::rule::Rule>
    parse_rules(std::vector<std::string> raw_rule_vector) override;

};


#endif // TEST_SIMPLE_PARSER_H