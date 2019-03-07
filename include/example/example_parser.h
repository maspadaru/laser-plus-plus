//
// Created by mike on 6/21/18.
//

#ifndef LASER_EXAMPLE_EXAMPLE_PARSER_H
#define LASER_EXAMPLE_EXAMPLE_PARSER_H

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <util/grounding.h>
#include <util/format_exception.h>
#include <util/timeline.h>

namespace laser {
namespace example {

enum class TokenType {
    OPERATOR,
    OPEN_PARENTHESES,
    CLOSED_PARENTHESES,
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

class ExampleParser {
  private:
    uint64_t current_tuple_counter = 0;

    std::vector<std::shared_ptr<laser::util::Grounding>> argument_stack;

    Token recognize(std::string token_string) const;

    std::vector<Token> tokenize(std::string const &rule_string) const;

    void add_token_attempt(std::vector<Token> &token_vector,
                           std::ostringstream &token_stream) const;

    void add_new_token(std::vector<Token> &token_vector, TokenType type,
                       char value_char) const;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    parse_token_vector(laser::util::Timeline const &timeline,
                       std::vector<Token> const &input_token_vector);

  public:
    ~ExampleParser() = default;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    parse_data(laser::util::Timeline &timeline,
               std::vector<std::string> const &raw_data_vector);
};

} // namespace example 
} // namespace laser

#endif // LASER_EXAMPLE_EXAMPLE_PARSER_H
