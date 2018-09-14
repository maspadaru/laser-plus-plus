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

static inline void rtrim(std::string *s);

static inline void ltrim(std::string *s);

static inline void trim(std::string *s);

static inline void syntax_error(std::string error_message);

namespace laser {
namespace rule {

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

/**
 *
 */
class DefaultRuleReader : public laser::rule::RuleReader {
private:
    std::string rule_string;
    std::stack<laser::formula::Formula *> argument_stack;
// Helper functions:
    inline void rtrim(std::string *s) const;
    inline void ltrim(std::string *s) const;
    inline void trim(std::string *s) const;
    inline void syntax_error(std::string const &error_message) const;

// methods

    bool is_unary_operator(Token token) const;

    bool is_binary_operator(Token token) const;

    Token recognize(std::string token_string) const;

    std::vector<Token> add_token_attempt(
            std::vector<Token> token_vector,
            std::ostringstream &token_stream
    ) const;

    std::vector<Token> add_new_token(
            std::vector<Token> token_vector,
            TokenType type, char value_char) const;

    std::vector<Token> tokenize(std::string rule_string) const;

    void parse_operator(Token token, bool is_head = false);

    std::tuple<size_t, std::vector<std::string>> parse_predicate_arguments(
            size_t index, std::vector<Token> *tokens) const;

    rule::Rule parse_token_vector(std::vector<Token> token_vector);

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
