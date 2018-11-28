//
// Created by mike on 6/21/18.
//

#ifndef BENCHAPP_FILE_PARSER_H
#define BENCHAPP_FILE_PARSER_H


#include <tuple>
#include <vector>
#include <algorithm>
#include <stack>
#include <sstream>
#include <utility>
#include <string>

#include <util/format_exception.h>
#include <util/data_atom.h>

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

class FileParser {
private:

    std::vector<laser::util::DataAtom> argument_stack;

    Token recognize(std::string token_string) const;

    std::vector<Token> tokenize(std::string const &rule_string) const;

    std::vector<Token> add_token_attempt(
            std::vector<Token> token_vector,
            std::ostringstream &token_stream
    ) const;

    std::vector<Token> add_new_token(
            std::vector<Token> token_vector,
            TokenType type, char value_char) const;

    std::vector<laser::util::DataAtom>
    parse_token_vector(std::vector<Token> input_token_vector);

public:

    ~FileParser() = default;

    std::vector<laser::util::DataAtom>
    parse_data(std::vector<std::string> raw_data_vector);

};


#endif // BENCHAPP_FILE_PARSER_H
