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
#include <string>

#include <excetion/format_exception.h>
#include <io/data_atom.h>

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

static inline void syntax_error(std::string error_message);

class SimpleParser {
private:

    std::vector<laser::io::DataAtom> argument_stack;

    Token recognize(std::string token_string) const;

    std::vector<Token> tokenize(std::string rule_string) const;

    std::vector<Token> add_token_attempt(
            std::vector<Token> token_vector,
            std::ostringstream &token_stream
    ) const;

    std::vector<Token> add_new_token(
            std::vector<Token> token_vector,
            TokenType type, char value_char) const;

    bool is_unary_operator(Token token) const;

    bool is_binary_operator(Token token) const;

    void parse_operator(Token token, bool is_head = false);

    std::tuple<size_t, std::vector<std::string>> parse_predicate_arguments(
            size_t index, std::vector<Token> *tokens) const;

    std::tuple<laser::formula::PseudoFormula, std::vector<laser::formula::PseudoFormula>>
    parse_token_vector(std::vector<Token> token_vector);

public:
    ~SimpleParser() = default;

    std::vector<laser::io::DataAtom>
    parse_data(std::vector<std::string> raw_data_vector);

};


#endif // TEST_SIMPLE_PARSER_H