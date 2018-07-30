//
// Created by mike on 7/17/18.
//

#include <formula/extended/atom.h>
#include "excetion/format_exception.h"
#include "rule/default_rule_reader.h"


namespace laser {
namespace rule {

DefaultRuleReader::DefaultRuleReader(
        std::vector<std::string>
        rule_string_vector) :
        rule_string_vector(std::move(rule_string_vector)) {}


std::vector<Rule> DefaultRuleReader::get_rules() {
    std::vector<Rule> rule_vector;
    for (const auto &rule_string : rule_string_vector) {
        auto token_vector = tokenize(rule_string);
        rule_vector.push_back(parse_token_vector(std::move(token_vector)));
    }
    return rule_vector;
}

// === Tokenizer ===

Token DefaultRuleReader::recognize(std::string token_string) const {
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

std::vector<Token> DefaultRuleReader::add_new_token(
        std::vector<Token> token_vector,
        TokenType type, char value_char) const {
    Token token;
    token.type = type;
    std::string value_string(1, value_char);
    token.value = value_string;
    token_vector.push_back(token);
    return token_vector;
}

std::vector<Token> DefaultRuleReader::add_token_attempt(
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

std::vector<Token> DefaultRuleReader::tokenize(std::string rule_string) const {
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

bool DefaultRuleReader::is_unary_operator(Token token) const {
    auto value = token.value;
    return (value == "@") ||
            (value == "not") ||
            (value == "box") ||
            (value == "diamond") ||
            (value == "time_win") ||
            (value == "tuple_win");

}

bool DefaultRuleReader::is_binary_operator(Token token) const {
    auto value = token.value;
    return (value == "and") || (value == "or");
}


std::tuple<size_t, std::vector<std::string>>
DefaultRuleReader::parse_predicate_arguments(
        size_t index, std::vector<Token> *tokens) const {
    std::vector<std::string> arguments;
    if (index > tokens->size()) {
        syntax_error("Expected constant, or variable after token '('");
    }
    while (index < tokens->size()) {
        Token token = tokens->at(index);
        if (token.type == TokenType::IDENTIFIER) {
            arguments.push_back(token.value);
            size_t next_index = index + 1;
            if (next_index < tokens->size()) {
                Token next_token = tokens->at(next_index);
                if (next_token.value == ",") {
                    index++;
                }
            } else {
                syntax_error("Expected ',' or ')' after '" + token.value);
            }
        } else if (token.type == TokenType::CLOSED_PARENTHESES) {
            if (arguments.empty()) {
                syntax_error("Expected at least one constant, or variable "
                             "after token '('");
            }
            break;
        }
        index++;
    }
    return std::make_tuple(index, arguments);
}

void DefaultRuleReader::parse_operator(Token token, bool is_head) {
    if (token.value == "and" || token.value == ",") {
        if (argument_stack.size() < 2) {
            syntax_error("Expected two operands for JOIN operation");
        }
        formula::Formula* operand1 =
                argument_stack.top();
        argument_stack.pop();
        formula::Formula* operand2 =
                argument_stack.top();
        argument_stack.pop();
        // TODO Formula *conjunction = new conjunction(operand1, operand2);
        formula::Formula *conjunction = new formula::Atom("placeholder");
        argument_stack.push(conjunction);
    }
}

rule::Rule
DefaultRuleReader::parse_token_vector(
        std::vector<Token> token_vector) {

    std::stack<Token> token_stack;
    formula::Formula* head;
    formula::Formula* body;
    argument_stack = std::stack<formula::Formula*>();
    size_t index = 0;
    while (index < token_vector.size()) {
        Token token = token_vector[index];
        switch (token.type) {
            case TokenType::OPEN_PARENTHESES : {
                // open parenthesis always has the highest precedence
                token_stack.push(token);
                break;
            }
            case TokenType::OPERATOR : {
                /* Since left has higher precedence than right,
                 * when we see an operator token, we must try to make
                 * sure all previously pushed operators on the stack are
                 * fully parsed before we push any new operator on the stack.
                 * So, we go through previous operators, and
                 * try to parse them if we have enough information now. */
                while (!token_stack.empty()) {
                    auto last_operator = token_stack.top();
                    if (is_unary_operator(last_operator) &&
                            is_unary_operator(token)) {
                        /* It must never happen that two unary operators come
                         * immediately after each other, without any binary
                         * operator between them. They can nest inside one
                         * another, but they cannot appear in the same level,
                         * and immediately after each other.
                         * That is a syntax error if happens. */
                        syntax_error(("error near operator", token.value, "!"));
                    } else if ((last_operator.type == TokenType::OPERATOR) &&
                            is_binary_operator(token)) {
                        /* If a binary, or unary operator is already on top of
                         * the stack, and another binary operator shows up, we
                         * must first finish the parsing of operator on the
                         * stack, and then deal with the new operator */
                        token_stack.pop();
                        parse_operator(last_operator);
                    } else {
                        /*If top of stack is occupied with coma, and/or
                         * parenthesis, that means we are still parsing
                         * argument_vector of an operator.
                         * In such cases, we cannot empty the stack, because
                         * current parsing is not done yet.
                         * We need to look into next tokens.
                         * So, we break the loop and continue to receive
                         * future tokens. */
                        break;
                    }
                }
                token_stack.push(token);
                break;
            }
            case TokenType::IDENTIFIER : {
                size_t next_index = index + 1;
                if (next_index < token_vector.size()) {
                    Token next_token = token_vector[next_index];
                    if (next_token.type == TokenType::OPEN_PARENTHESES) {
                        size_t first_argument_index = index + 2;
                        std::vector<std::string> arguments;
                        std::tie(index, arguments) =
                                parse_predicate_arguments(first_argument_index,
                                        &token_vector);
                        std::string predicate = token.value;
                        if (predicate == "MATH") {
                            if (arguments.size() != 4) {
                                syntax_error(("Expected four parameters for "
                                              "function MATH"));
                            }
                            // TODO argument_stack.push()
                        } else if (predicate == "COMP") {
                            if (arguments.size() != 3) {
                                syntax_error(("Expected three parameters for "
                                              "function COMP"));
                                // TODO argument_stack.push()
                            }
                        } else {
//                            auto atom = new formula::Atom(predicate,
//                                    argument_vector);
                            formula::Formula *atom =
                                    new formula::Atom(predicate, arguments);
                            argument_stack.push(atom);
                        }
                    }

                } else {
                    std::string predicate = token.value;
                    std::vector<std::string> arguments;
//                    auto atom = new formula::Atom(predicate, argument_vector);
                    formula::Formula *atom =
                            new formula::Atom(predicate, arguments);
                    argument_stack.push(atom);
                }
                break;
            }
            case TokenType::CLOSED_PARENTHESES : {
                bool keep_going = true;
                while (keep_going) {
                    if (argument_stack.empty()) {
                        syntax_error("Expected operand or '('");
                    }
                    Token operator_token = token_stack.top();
                    token_stack.pop();
                    if (operator_token.type == TokenType::OPEN_PARENTHESES) {
                        keep_going = false;
                    }
                    parse_operator(operator_token);
                }
                break;
            }
            case TokenType::ENTAILMENT_SIGN : {
                if (!argument_stack.empty()) {
                    /*Before parsing the body of the rule we must make sure all operators in the
                     * head are dealt with. So, we go through the operator stack, and make sure
                     * that all of them are processed. */
                    while (!token_stack.empty()) {
                        Token operator_token = token_stack.top();
                        token_stack.pop();
                        parse_operator(operator_token, true);

                    }
                    head = argument_stack.top();
                    argument_stack.pop();
                }
            }
                break;
        }
        index++;
    }
    while (!token_stack.empty()) {
        Token operator_token = token_stack.top();
        token_stack.pop();
        if (operator_token.type == TokenType::OPEN_PARENTHESES) {
            syntax_error("Missing ')' in rule ");
        }
        parse_operator(operator_token);
    }
    body = argument_stack.top();
    argument_stack.pop();

    // TODO discard rule with empty head.

    return rule::Rule(head, body);
}

// === Helper functions ===

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

static inline void syntax_error(std::string const &error_message) {
    std::string message = "Syntax Error in Simple Parser: " + error_message;
    const char *exception_message = message.c_str();
    throw exception::FormatException(exception_message);

}

} // namespace rule
} // namespace laser
