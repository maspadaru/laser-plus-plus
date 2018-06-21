//
// Created by mike on 6/21/18.
//

#include <sstream>
#include <utility> #include "simple_parser.h"
#include <simple_parser.h>


#include <iostream> // DELETE THIS

/* trim from start (in place)
 * source:
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

/* trim from end (in place)
 * source:
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

/* trim from both ends (in place)
 * source:
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::tuple<int, std::unordered_map<std::string,
        std::vector<laser::formula::Formula * >>>
SimpleParser::parse_data(
        std::vector<std::string>
        raw_data_vector) const {
    return std::tuple<int, std::unordered_map<std::string,
            std::vector<laser::formula::Formula * >>>();
}

// === Tokenization ===

Token SimpleParser::recognize(std::string token_string) const {
    trim(token_string);
    Token token;
    token.value = token_string;
    if ((token_string == "@") ||
            (token_string == "and") ||
            (token_string == "or") ||
            (token_string == "not") ||
            (token_string == "box") ||
            (token_string == "diamond") ||
            (token_string == "time_win") ||
            (token_string == "tuple_win") ||
            (token_string == "predicate_win")) {
        token.type = TokenType::OPERATOR;
    } else if (token_string == ":-") {
        token.type = TokenType::ENTAILMENT_SIGN;
    } else {
        token.type = TokenType::IDENTIFIER;
    }
    return token;
}

std::vector<Token> SimpleParser::add_token_attempt(
        std::vector<Token> token_vector,
        std::ostringstream &token_stream) const {

    /* TODO:
     * What happens to these Token items created on the stack?
     * Will they be lost at the end of the function, and token_vector
     * will just have orphan references?
     */
    std::string str = token_stream.str();
    if (!str.empty()) {
        Token token = recognize(str);
        token_vector.push_back(token);
    }
    token_stream.clear();token_stream.str("");
    return token_vector;
}

std::vector<Token> SimpleParser::add_new_token(
        std::vector<Token> token_vector,
        TokenType type, char value_char) const {
    Token token;
    token.type = type;
    std::string value_string(1, value_char);
    token.value = value_string;
    token_vector.push_back(token);
    return token_vector;
}

std::vector<Token> SimpleParser::tokenize(std::string rule_string) const {
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
                add_new_token(token_vector, TokenType::OPEN_PARENTHESES, c);
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


// === Parse Rules ===

std::vector<laser::rule::Rule *>
SimpleParser::parse_rules(std::vector<std::string> raw_rule_vector) const {
    for (const auto &raw_rule_string : raw_rule_vector) {
        auto token_vector = tokenize(raw_rule_string);
        // laser::rule::Rule rule =
                parse_rule(token_vector);
    }
    return std::vector<laser::rule::Rule *>();
}

void SimpleParser::parse_rule(std::vector<Token> token_vector) const {
    for (auto token : token_vector) {
        std::cout << "Token: " << token.value << std::endl;
    }
}
