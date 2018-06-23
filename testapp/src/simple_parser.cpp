//
// Created by mike on 6/21/18.
//

#include <sstream>
#include <utility> #include "simple_parser.h"
#include <simple_parser.h>


#include <iostream> // DELETE THIS
#include <excetion/format_exception.h>

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

static inline Token pop(std::stack<Token> *stack) {
    auto result = stack->top();
    stack->pop();
    return result;
}

static inline void syntax_error(std::string error_message) {
    error_message = "Syntax Error in Simple Parser: ", error_message;
    const char *exception_message = error_message.c_str();
    throw laser::exception::FormatException(exception_message);

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
    trim(&token_string);
    Token token;
    token.value = token_string;
    if ((token_string == "@") ||
            (token_string == "and") ||
            (token_string == "or") ||
            (token_string == "not") ||
            (token_string == "box") ||
            (token_string == "diamond") ||
            (token_string == "time_win") ||
            (token_string == "tuple_win")) {
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
    token_stream.clear();
    token_stream.str("");
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

bool SimpleParser::is_unary_operator(Token token) const {
    auto value = token.value;
    return (value == "@") ||
            (value == "not") ||
            (value == "box") ||
            (value == "diamond") ||
            (value == "time_win") ||
            (value == "tuple_win");

}

bool SimpleParser::is_binary_operator(Token token) const {
    auto value = token.value;
    return (value == "and") || (value == "or");
}

std::vector<laser::rule::Rule>
SimpleParser::parse_rules(std::vector<std::string> raw_rule_vector) const {
    std::vector<laser::rule::Rule> rule_vector;
    for (const auto &raw_rule_string : raw_rule_vector) {
        auto token_vector = tokenize(raw_rule_string);
        // laser::rule::Rule rule =
                parse_rule(token_vector);
        // rule_vector.push_back(rule);
    }
    return rule_vector;
}

// laser::rule::Rule
void SimpleParser::parse_rule(
        std::vector<Token> token_vector) const {
    for (auto token : token_vector) {
        std::cout << "Token: " << token.value << std::endl;
    }

//    std::stack<Token> token_stack;
//    std::stack<laser::formula::Formula*> argument_stack;
//    laser::formula::Formula *head;
//    laser::formula::Formula *rule;
//    size_t index = 0;
//    while (index < token_vector.size()) {
//        Token token = token_vector[index];
//        switch (token.type) {
//            case TokenType::OPEN_PARENTHESES :
//                // open parenthesis always has the highest precedence
//                token_stack.push(token);
//            case TokenType::OPERATOR :
//                /* Since left has higher precedence than right,
//                 * when we see an operator token, we must try to make
//                 * sure all previously pushed operators on the stack are
//                 * fully parsed before we push any new operator on the stack.
//                 * So, we go through previous operators, and
//                 * try to parse them if we have enough information now. */
//                while (!token_stack.empty()) {
//                    auto last_operator = token_stack.top();
//                    if (is_unary_operator(last_operator) &&
//                            is_unary_operator(token)) {
//                        /* It must never happen that two unary operators come
//                         * immediately after each other, without any binary
//                         * operator between them. They can nest inside one
//                         * another, but they cannot appear in the same level,
//                         * and immediately after each other.
//                         * That is a syntax error if happens. */
//                        syntax_error(("error near operator", token.value, "!"));
//                    } else if ((last_operator.type == TokenType::OPERATOR) &&
//                            is_binary_operator(token)) {
//                        /* If a binary, or unary operator is already on top of
//                         * the stack, and another binary operator shows up, we
//                         * must first finish the parsing of operator on the
//                         * stack, and then deal with the new operator */
//                        token_stack.pop();
//                        parse_operator(last_operator, argument_stack);
//                    } else {
//                        /*If top of stack is occupied with coma, and/or
//                         * parenthesis, that means we are still parsing
//                         * arguments of an operator.
//                         * In such cases, we cannot empty the stack, because
//                         * current parsing is not done yet.
//                         * We need to look into next tokens.
//                         * So, we break the loop and continue to receive
//                         * future tokens. */
//                        break;
//                    }
//                }
//                token_stack.push(token);
//            case TokenType::IDENTIFIER :
//                size_t next_index = index + 1;
//                if (next_index < token_vector.size()) {
//                    Token next_token = token_vector[next_index];
//                    if (next_token.type == TokenType::OPEN_PARENTHESES) {
//                        size_t first_argument_index = index + 2;
//                        std::vector<std::string> test;
//                        std::string arguments;
//                        std::tie(index, arguments) =
//                                parse_predicate_arguments(first_argument_index,
//                                        argument_stack);
//                        std::string predicate = token.value;
//                        if (predicate == "MATH") {
//                            if (arguments.length() != 4) {
//                                syntax_error(("Expected four parameters for "
//                                              "function MATH"));
//                            }
//                                // TODO argument_stack.push()
//                        } else if (predicate == "COMP") {
//                            if (arguments.length() != 3) {
//                                syntax_error(("Expected three parameters for "
//                                              "function COMP"));
//                                // TODO argument_stack.push()
//                            }
//                        } else {
//                            auto atom = new laser::formula::Atom();
//                            argument_stack.push(Atom())
//                        }
//                    }
//
//                }
//
//        }
//    }
}

void SimpleParser::parse_operator(
        Token token, std::stack<laser::formula::Formula*> &stack) const {

}



std::tuple<size_t, std::string> SimpleParser::parse_predicate_arguments(
        size_t index, std::stack<laser::formula::Formula*> &stack) const {
    return std::tuple<size_t, std::string>();
}