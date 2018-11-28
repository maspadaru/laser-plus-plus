//
// Created by mike on 6/21/18.
//

#include "file_parser.h"

/* trim from start (in place)
 * source:
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline void ltrim(std::string *s) {
    s->erase(s->begin(), std::find_if(s->begin(), s->end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

/* trim from end (in place)
 * source:
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline void rtrim(std::string *s) {
    s->erase(std::find_if(s->rbegin(), s->rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s->end());
}

/* trim from both ends (in place)
 * source:
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline void trim(std::string *s) {
    ltrim(s);
    rtrim(s);
}

static inline void syntax_error(std::string error_message) {
    std::string message = "Syntax Error in File Parser: " + error_message;
    const char *exception_message = message.c_str();
    throw laser::util::FormatException(exception_message);

}

// === Tokenizer ===

Token FileParser::recognize(std::string token_string) const {
    trim(&token_string);
    Token token;
    token.value = token_string;
    if (token_string == ",") {
        token.type = TokenType::OPERATOR;
    } else {
        token.type = TokenType::IDENTIFIER;
    }
    return token;
}

std::vector<Token> FileParser::add_token_attempt(
        std::vector<Token> token_vector,
        std::ostringstream &token_stream) const {
    std::string str = token_stream.str();
    if (!str.empty()) {
        Token token = recognize(str);
        token_vector.push_back(token);
    }
    token_stream.clear();
    token_stream.str("");
    return token_vector;
}

std::vector<Token> FileParser::add_new_token(
        std::vector<Token> token_vector,
        TokenType type, char value_char) const {
    Token token;
    token.type = type;
    std::string value_string(1, value_char);
    token.value = value_string;
    token_vector.push_back(token);
    return token_vector;
}

std::vector<Token> FileParser::tokenize(std::string const &rule_string) const {
    std::vector<Token> token_vector;
    std::istringstream rule_stream(rule_string);
    std::ostringstream token_stream;
    char c;
    while (rule_stream >> c) {
        if (c == ',') {
            token_vector = add_token_attempt(token_vector, token_stream);
            token_vector = add_new_token(token_vector, TokenType::OPERATOR,
                    c);

        } else if (c == '(') {
            token_vector = add_token_attempt(token_vector, token_stream);
            token_vector =
                    add_new_token(token_vector, TokenType::OPEN_PARENTHESES, c);
        } else if (c == ')') {
            token_vector = add_token_attempt(token_vector, token_stream);
            token_vector =
                    add_new_token(token_vector, TokenType::CLOSED_PARENTHESES,
                            c);
        } else {
            token_stream << c;
        }
        int stream_peek = rule_stream.peek();
        if (stream_peek == ' ') {
            token_vector = add_token_attempt(token_vector, token_stream);
        }
    }
    token_vector = add_token_attempt(token_vector, token_stream);
    return token_vector;
}

// ======== Parser =========


std::vector<laser::util::DataAtom>
FileParser::parse_token_vector(
        std::vector<Token> input_token_vector) {
    std::vector<std::string> token_list;
    std::string predicate = input_token_vector[0].value;
    std::vector<laser::util::DataAtom> result;
    for (int index = 1; index < input_token_vector.size(); index++) {
        Token token = input_token_vector[index];
        switch (token.type) {
            case TokenType::OPERATOR : {
                // TODO: ignore for now, use later for syntax check
                break;
            }
            case TokenType::IDENTIFIER : {
                token_list.push_back(token.value);
                break;
            }
        }
    }
    result.emplace_back(predicate, token_list);
    token_list.clear();
    return result;
}

std::vector<laser::util::DataAtom>
FileParser::parse_data(
        std::vector<std::string>
        raw_data_vector) {
    std::vector<laser::util::DataAtom> data_atom_vector;
    for (const auto &raw_string : raw_data_vector) {
        auto token_vector = tokenize(raw_string);
        if (!token_vector.empty()) {
            auto temp_vector =
                    parse_token_vector(std::move(token_vector));
            data_atom_vector
                    .insert(data_atom_vector.end(), temp_vector.begin(),
                            temp_vector.end());
        }
    }
    return data_atom_vector;
}



