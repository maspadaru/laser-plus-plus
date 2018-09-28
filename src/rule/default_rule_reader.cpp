//
// Created by mike on 7/17/18.
//

#include "rule/default_rule_reader.h"

namespace laser {
namespace rule {

DefaultRuleReader::DefaultRuleReader(std::string rule_string)
    : rule_string(std::move(rule_string)) {}

inline void
DefaultRuleReader::syntax_error(std::string const &error_message) const {
    std::string message = "Line: " + std::to_string(line_counter) +
                          " -> Syntax Error: " + error_message;
    const char *exception_message = message.c_str();
    throw util::FormatException(exception_message);
}

void DefaultRuleReader::skip_spaces() {
    input >> std::ws; // eats-up any leadin whitespaces.
}

void DefaultRuleReader::skip_next_char() { input.get(); }
char DefaultRuleReader::read_next_char() { return input.get(); }

void DefaultRuleReader::skip_expected_char(char c) {
    char input_char = read_next_char();
    if (input_char != c) {
        std::stringstream error_stream;
        error_stream << "Expected character [" << c 
            << "] but actual caracter was ["
            << input_char << "].";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
}

char DefaultRuleReader::peek_next_char() { return input.peek(); }

bool DefaultRuleReader::is_next_char_letter() {
    return std::isalpha(peek_next_char()) > 0;
}

bool DefaultRuleReader::is_next_char_digit() {
    return std::isdigit(peek_next_char()) > 0;
}

bool DefaultRuleReader::is_next_char_letter_or_digit() {
    return is_next_char_letter() || is_next_char_digit();
}

bool DefaultRuleReader::is_next_char(char c) { return peek_next_char() == c; }

bool DefaultRuleReader::is_next_char_binary_operator() {
    return is_next_char('&') || is_next_char('|');
}

bool DefaultRuleReader::is_next_char_math_operator() {
    return is_next_char('+') || is_next_char('-') || is_next_char('*') ||
           is_next_char('/');
}

bool DefaultRuleReader::is_next_char_comparison_operator() {
    return is_next_char('<') || is_next_char('>') || is_next_char('=') ||
           is_next_char('!');
}

void DefaultRuleReader::parse_eoln() {
    skip_spaces();
    if (!input.eof()) {
        syntax_error("Expecting EOL");
    }
}

std::string DefaultRuleReader::parse_math_sign() {
    // TODO
    return "";
}

std::string DefaultRuleReader::parse_comparison_sign() {
    // TODO
    return "";
}

char DefaultRuleReader::parse_nonzero() {
    char result = read_next_char();
    if(std::isdigit(result) == 0 || result == '0') {
        syntax_error("Expected non-zero digit");
    }
    return result;
}

std::string DefaultRuleReader::parse_zero() {
    skip_next_char();
    return "0";
}

char DefaultRuleReader::parse_diggit() {
    char result = read_next_char();
    if(std::isdigit(result) == 0) {
        syntax_error("Expected digit");
    }
    return result;
}

char DefaultRuleReader::parse_letter() {
    char input_char = read_next_char();
    if(std::isalpha(input_char) == 0) {
        std::stringstream error_stream;
        error_stream << "Expected letter " 
            << " but actual caracter was ["
            << input_char << "], hex=" 
            << std::hex << (unsigned short int) input_char  << ".";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
    return input_char;
}

std::string DefaultRuleReader::parse_positive_number() {
    std::stringstream result_stream;
    result_stream << parse_nonzero();
    while(is_next_char_digit()) {
        result_stream << read_next_char(); 
    }
    return result_stream.str();
}

std::string DefaultRuleReader::parse_natural_number() {
    if (is_next_char('0')) {
        return parse_zero();
    }
    return parse_positive_number();
}

std::string DefaultRuleReader::parse_integer() {
    if (is_next_char('0')) {
        return parse_zero();
    }
    std::stringstream result_stream;
    if (is_next_char('-')) {
        result_stream << read_next_char();
        skip_spaces();
    }
    result_stream << parse_positive_number();
    return result_stream.str();
}

std::string DefaultRuleReader::parse_float() {
    std::stringstream result_stream;
    result_stream << parse_integer();
    if (is_next_char('.')) {
        skip_next_char();
        do {
            result_stream << parse_diggit();
        } while(is_next_char_digit());
    }
    return result_stream.str();
}

std::string DefaultRuleReader::parse_identifier() {
    std::stringstream result_stream;
    result_stream << parse_letter();
    while(is_next_char_letter_or_digit()) {
        result_stream << read_next_char(); 
    }
    return result_stream.str();
}

std::string DefaultRuleReader::parse_argument() {
    if(is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_integer();
}

std::string DefaultRuleReader::parse_float_argument() {
    if(is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_float();
}

std::vector<laser::rule::Rule> DefaultRuleReader::parse_program() {
    std::vector<laser::rule::Rule> result;
    std::stringstream program_stream;
    program_stream.str(rule_string);
    std::string line;
    line_counter = 0;
    while (std::getline(program_stream, line, '\n')) {
        line_counter++;
        input.clear();
        input.str(line);
        auto rule = parse_rule();
        result.push_back(rule);
    }
    return result;
}

laser::rule::Rule DefaultRuleReader::parse_rule() {
    auto head = parse_head();
    skip_spaces();
    skip_expected_char(':');
    skip_expected_char('=');
    auto body = parse_body();
    parse_eoln();
    auto result = laser::rule::Rule(head, body);
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_head() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('[')) {
        skip_next_char();
        result = parse_exact_time();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_body() {
    return parse_formula();
}

laser::formula::Formula *DefaultRuleReader::parse_formula() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_complex_formula();
    } else {
        result = parse_binary_formula();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_complex_formula() {
    skip_expected_char('(');
    auto result = parse_formula();
    skip_spaces();
    skip_expected_char(')');
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_binary_formula() {
    auto left_term = parse_term();
    skip_spaces();
    while (is_next_char_binary_operator()) {
        auto binary_formula = parse_binary_operator();
        auto right_term = parse_term();
        binary_formula->add_child(left_term);
        binary_formula->add_child(right_term);
        left_term = binary_formula;
        skip_spaces();
    }
    return left_term;
}

laser::formula::Formula *DefaultRuleReader::parse_binary_operator() {
    laser::formula::Formula *result;
    if (is_next_char('&')) {
        skip_expected_char('&');
        skip_expected_char('&');
        // result = new Conjunction();
    } else {
        skip_expected_char('|');
        skip_expected_char('|');
        // result = new Disjunction();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_term() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_complex_formula();
    } else if (is_next_char('[')) {
        result = parse_unary_formula();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_atom() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char_math_operator()) {
        result = parse_math_atom();
    } else if (is_next_char_comparison_operator()) {
        result = parse_comparison_atom();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_predicate_atom() {
    std::vector<std::string> argument_vector;
    std::string predicate = parse_identifier();
    skip_spaces();
    if (is_next_char('(')) {
        skip_next_char();
        skip_spaces();
        if(is_next_char_letter_or_digit()) {
            std::string argument = parse_float_argument();
            argument_vector.push_back(argument);
            skip_spaces();
            while (is_next_char(',')) {
                skip_next_char();
                skip_spaces();
                std::string argument = parse_float_argument();
                argument_vector.push_back(argument);
                skip_spaces();
            }
        }
        skip_expected_char(')');
    }
    return new laser::formula::Atom(predicate, argument_vector);
}

laser::formula::Formula *DefaultRuleReader::parse_comparison_atom() {
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *DefaultRuleReader::parse_math_atom() {
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *DefaultRuleReader::parse_unary_formula() {
    laser::formula::Formula *result;
    skip_expected_char('[');
    skip_spaces();
    char unary_operator = peek_next_char();
    switch (unary_operator) {
    case 'D':
        result = parse_diamond();
        break;
    case 'B':
        result = parse_box();
        break;
    case '!':
        result = parse_negation();
        break;
    case '@':
        result = parse_exact_time();
        break;
    case '$':
        result = parse_time_window();
        break;
    case '#':
        result = parse_tuple_window();
        break;
    default :
        syntax_error("Not a valid unary operator");
    }
    return result;
}

laser::formula::Formula *DefaultRuleReader::parse_diamond() {
    skip_expected_char('D');
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_formula();
    return new laser::formula::Diamond(child);
}

laser::formula::Formula *DefaultRuleReader::parse_box() {
    // skip_expected_char("B");
    // skip_spaces();
    // skip_expected_char("]");
    // auto child = parse_formula();
    // return new Box(child);
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *DefaultRuleReader::parse_negation() {
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *DefaultRuleReader::parse_exact_time() {
    // skip_expected_char("@");
    // skip_spaces();
    // skip_expected_char(",");
    // skip_spaces();
    // std::string argument = parse_argument();
    // skip_spaces();
    // skip_expected_char("]");
    // auto child = parse_formula();
    // return new laser::Formula::Exact_Time(argument, child);
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *DefaultRuleReader::parse_time_window() {
    skip_expected_char('$');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_natural_number();
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_formula();
    uint64_t window_size = std::stoull(argument);  
    return new laser::formula::TimeWindow(window_size, child);
}

laser::formula::Formula *DefaultRuleReader::parse_tuple_window() {
    // TODO
    return new laser::formula::Atom("TODO");
}

std::vector<laser::rule::Rule> DefaultRuleReader::get_rules() {
    return parse_program();
}

} // namespace rule
} // namespace laser
