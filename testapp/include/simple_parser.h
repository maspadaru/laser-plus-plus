//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_PARSER_H
#define TEST_SIMPLE_PARSER_H


#include <tuple>
#include <vector>
#include <algorithm>
#include <stack>

#include <input/data_parser.h>
#include <input/rule_parser.h>
#include <rule/rule.h>
#include <formula/formula.h>

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

// Helper functions:

static inline void rtrim(std::string *s);

static inline void ltrim(std::string *s);

static inline void trim(std::string *s);

static inline Token pop(std::stack<Token> *stack);

static inline void syntax_error(std::string error_message);


class SimpleParser
        : public laser::input::DataParser, public laser::input::RuleParser {
private:

    Token recognize(std::string token_string) const;

    std::vector<Token> tokenize(std::string rule_string) const;

    std::vector<Token> add_token_attempt(
            std::vector<Token> token_vector,
            std::ostringstream &token_stream
    ) const;

    std::vector<Token> add_new_token(
            std::vector<Token> token_vector,
            TokenType type, char value_char) const;

    // laser::rule::Rule
    void parse_rule(std::vector<Token> token_vector) const;

    bool is_unary_operator(Token token) const;

    bool is_binary_operator(Token token) const;

    void parse_operator(Token token, std::stack<laser::formula::Formula*> &stack) const;

    std::tuple<size_t, std::string> parse_predicate_arguments(
            size_t index, std::stack<laser::formula::Formula*> &stack) const;

public:
    ~SimpleParser() override = default;

    std::tuple<int, std::unordered_map<std::string, std::vector<laser::formula::Formula *>>>
    parse_data(std::vector<std::string> raw_data_vector) const override;

    std::vector<laser::rule::Rule>
    parse_rules(std::vector<std::string> raw_rule_vector) const override;
};


#endif // TEST_SIMPLE_PARSER_H