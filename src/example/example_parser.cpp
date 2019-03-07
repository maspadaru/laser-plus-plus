//
// Created by mike on 6/21/18.
//

#include "example/example_parser.h"

namespace laser {
namespace example {

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
    s->erase(std::find_if(s->rbegin(), s->rend(),
                          [](int ch) { return !std::isspace(ch); })
                 .base(),
             s->end());
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
    std::string message = "Syntax Error in Example Parser: " + error_message;
    const char *exception_message = message.c_str();
    throw laser::util::FormatException(exception_message);
}

// === Tokenizer ===

Token ExampleParser::recognize(std::string token_string) const {
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

void ExampleParser::add_token_attempt(std::vector<Token> &token_vector,
                                      std::ostringstream &token_stream) const {
    std::string str = token_stream.str();
    if (!str.empty()) {
        Token token = recognize(std::move(str));
        token_vector.push_back(token);
    }
    token_stream.clear();
    token_stream.str("");
}

void ExampleParser::add_new_token(std::vector<Token> &token_vector,
                                  TokenType type, char value_char) const {
    Token token;
    token.type = type;
    std::string value_string(1, value_char);
    token.value = value_string;
    token_vector.push_back(token);
}

std::vector<Token>
ExampleParser::tokenize(std::string const &rule_string) const {
    std::vector<Token> token_vector;
    std::istringstream rule_stream(rule_string);
    std::ostringstream token_stream;
    char c;
    while (rule_stream >> c) {
        if (c == ',') {
            add_token_attempt(token_vector, token_stream);
            add_new_token(token_vector, TokenType::OPERATOR, c);

        } else if (c == '(') {
            add_token_attempt(token_vector, token_stream);
            add_new_token(token_vector, TokenType::OPEN_PARENTHESES, c);
        } else if (c == ')') {
            add_token_attempt(token_vector, token_stream);
            add_new_token(token_vector, TokenType::CLOSED_PARENTHESES, c);
        } else {
            token_stream << c;
        }
        int stream_peek = rule_stream.peek();
        if (stream_peek == ' ') {
            add_token_attempt(token_vector, token_stream);
        }
    }
    add_token_attempt(token_vector, token_stream);
    return token_vector;
}

// ======== Parser =========

std::vector<std::shared_ptr<laser::util::Grounding>>
ExampleParser::parse_token_vector(
    laser::util::Timeline const &timeline,
    std::vector<Token> const &input_token_vector) {
    uint64_t time = timeline.get_time();
    uint64_t max_tuple_counter = laser::util::Timeline::INFINITE_TIME;
    std::vector<std::string> token_list;
    std::string predicate;
    std::vector<std::shared_ptr<laser::util::Grounding>> result;
    for (auto token : input_token_vector) {
        switch (token.type) {
        case TokenType::OPEN_PARENTHESES: {
            // TODO: check there is exactly one element in token_list
            predicate = token_list.at(0);
            token_list.clear();
            break;
        }
        case TokenType::OPERATOR: {
            // TODO: ignore for now, use later for syntax check
            break;
        }
        case TokenType::IDENTIFIER: {
            token_list.push_back(token.value);
            break;
        }
        case TokenType::CLOSED_PARENTHESES: {
            current_tuple_counter++;
            auto grounding = std::make_shared<laser::util::Grounding>(
                predicate, time, time, current_tuple_counter, max_tuple_counter,
                true, false, std::move(token_list));
            result.push_back(std::move(grounding));
            token_list.clear();
            break;
        }
        }
    }
    return result;
}

std::vector<std::shared_ptr<laser::util::Grounding>>
ExampleParser::parse_data(laser::util::Timeline &timeline,
                          std::vector<std::string> const &raw_data_vector) {
    std::vector<std::shared_ptr<laser::util::Grounding>> grounding_vector;
    for (const auto &raw_string : raw_data_vector) {
        auto token_vector = tokenize(raw_string);
        if (!token_vector.empty()) {
            auto temp_vector = parse_token_vector(timeline, token_vector);
            grounding_vector.insert(
                grounding_vector.end(),
                std::make_move_iterator(temp_vector.begin()),
                std::make_move_iterator(temp_vector.end()));
        }
    }
    timeline.set_tuple_count(current_tuple_counter);
    return grounding_vector;
}

} // namespace example 
} // namespace laser
