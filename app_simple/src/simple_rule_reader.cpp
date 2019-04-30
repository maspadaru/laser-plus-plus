#include "simple_rule_reader.h"

SimpleRuleReader::SimpleRuleReader(std::string const &stream_path) {
    program_stream.open(stream_path);
}

SimpleRuleReader::~SimpleRuleReader() { program_stream.close(); }

inline void
SimpleRuleReader::syntax_error(std::string const &error_message) const {
    std::string message = "Line: " + std::to_string(line_counter) +
                          " -> Syntax Error: " + error_message;
    const char *exception_message = message.c_str();
    throw laser::util::FormatException(exception_message);
}

void SimpleRuleReader::skip_spaces() {
    input >> std::ws; // eats-up any leadin whitespaces.
}

void SimpleRuleReader::skip_next_char() { input.get(); }
char SimpleRuleReader::read_next_char() { return input.get(); }

void SimpleRuleReader::skip_expected_char(char c) {
    char input_char = read_next_char();
    if (input_char != c) {
        std::stringstream error_stream;
        error_stream << "Expected character [" << c
                     << "] but actual caracter was [" << input_char << "].";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
}

char SimpleRuleReader::peek_next_char() { return input.peek(); }

bool SimpleRuleReader::is_next_char_letter() {
    return std::isalpha(peek_next_char()) > 0;
}

bool SimpleRuleReader::is_next_char_digit() {
    return std::isdigit(peek_next_char()) > 0;
}

bool SimpleRuleReader::is_next_char_letter_or_digit() {
    return is_next_char_letter() || is_next_char_digit();
}

bool SimpleRuleReader::is_next_char(char c) { return peek_next_char() == c; }

bool SimpleRuleReader::is_next_char_binary_operator() {
    return is_next_char('&') || is_next_char('|');
}

bool SimpleRuleReader::is_next_char_math_operator() {
    return is_next_char('+') || is_next_char('-') || is_next_char('*') ||
           is_next_char('/');
}

bool SimpleRuleReader::is_next_char_comparison_operator() {
    return is_next_char('<') || is_next_char('>') || is_next_char('=') ||
           is_next_char('!');
}

void SimpleRuleReader::parse_eoln() {
    skip_spaces();
    if (!input.eof()) {
        syntax_error("Expecting EOL");
    }
}

std::string SimpleRuleReader::parse_math_sign() {
    // TODO
    return "";
}

std::string SimpleRuleReader::parse_comparison_sign() {
    // TODO
    return "";
}

char SimpleRuleReader::parse_nonzero() {
    char result = read_next_char();
    if (std::isdigit(result) == 0 || result == '0') {
        syntax_error("Expected non-zero digit");
    }
    return result;
}

std::string SimpleRuleReader::parse_zero() {
    skip_next_char();
    return "0";
}

char SimpleRuleReader::parse_diggit() {
    char result = read_next_char();
    if (std::isdigit(result) == 0) {
        syntax_error("Expected digit");
    }
    return result;
}

char SimpleRuleReader::parse_letter() {
    char input_char = read_next_char();
    if (std::isalpha(input_char) == 0) {
        std::stringstream error_stream;
        error_stream << "Expected letter "
                     << " but actual caracter was [" << input_char
                     << "], hex=" << std::hex << (uint16_t)input_char << ".";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
    return input_char;
}

std::string SimpleRuleReader::parse_positive_number() {
    std::stringstream result_stream;
    result_stream << parse_nonzero();
    while (is_next_char_digit()) {
        result_stream << read_next_char();
    }
    return result_stream.str();
}

std::string SimpleRuleReader::parse_natural_number() {
    if (is_next_char('0')) {
        return parse_zero();
    }
    return parse_positive_number();
}

std::string SimpleRuleReader::parse_integer() {
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

std::string SimpleRuleReader::parse_float() {
    std::stringstream result_stream;
    result_stream << parse_integer();
    if (is_next_char('.')) {
        skip_next_char();
        do {
            result_stream << parse_diggit();
        } while (is_next_char_digit());
    }
    return result_stream.str();
}

std::string SimpleRuleReader::parse_identifier() {
    std::stringstream result_stream;
    result_stream << parse_letter();
    while (is_next_char_letter_or_digit()) {
        result_stream << read_next_char();
    }
    return result_stream.str();
}

std::string SimpleRuleReader::parse_argument() {
    if (is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_integer();
}

std::string SimpleRuleReader::parse_float_argument() {
    if (is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_float();
}

bool SimpleRuleReader::read_line(std::string &line) {
    std::getline(program_stream, line, '\n');
    return program_stream.good();
}

std::vector<laser::rule::Rule> SimpleRuleReader::parse_program() {
    std::vector<laser::rule::Rule> result;
    std::string line;
    line_counter = 0;
    while (read_line(line)) {
        line_counter++;
        input.clear();
        input.str(line);
        auto rule = parse_rule();
        result.push_back(rule);
    }
    return result;
}

laser::rule::Rule SimpleRuleReader::parse_rule() {
    auto head = parse_head();
    skip_spaces();
    skip_expected_char(':');
    skip_expected_char('=');
    auto body = parse_body();
    parse_eoln();
    auto result = laser::rule::Rule(body, head);
    return result;
}

std::vector<laser::formula::Formula *> SimpleRuleReader::parse_head() {
    return parse_formula_vector();
}

laser::formula::Formula *SimpleRuleReader::parse_body() {
    return parse_formula();
}

laser::formula::Formula *SimpleRuleReader::parse_formula() {
    laser::formula::Formula *result;
    skip_spaces();
    if (is_next_char('(')) {
        result = parse_complex_formula();
    } else {
        result = parse_binary_formula();
    }
    return result;
}

laser::formula::Formula *SimpleRuleReader::parse_complex_formula() {
    skip_expected_char('(');
    auto result = parse_formula();
    skip_spaces();
    skip_expected_char(')');
    return result;
}

laser::formula::Formula *SimpleRuleReader::parse_binary_formula() {
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

laser::formula::Formula *SimpleRuleReader::parse_binary_operator() {
    laser::formula::Formula *result;
    if (is_next_char('&')) {
        skip_expected_char('&');
        skip_expected_char('&');
        result = new laser::formula::Conjunction();
    } else {
        skip_expected_char('|');
        skip_expected_char('|');
        // result = new Disjunction();
    }
    return result;
}

laser::formula::Formula *SimpleRuleReader::parse_term() {
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

laser::formula::Formula *SimpleRuleReader::parse_atom() {
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

laser::formula::Formula *SimpleRuleReader::parse_predicate_atom() {
    std::vector<std::string> argument_vector;
    std::string predicate = parse_identifier();
    skip_spaces();
    if (is_next_char('(')) {
        skip_next_char();
        skip_spaces();
        if (is_next_char_letter_or_digit()) {
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
    return new laser::formula::Atom(predicate, std::move(argument_vector));
}

laser::formula::Formula *SimpleRuleReader::parse_comparison_atom() {
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *SimpleRuleReader::parse_math_atom() {
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *SimpleRuleReader::parse_unary_formula() {
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
        result = parse_time_reference();
        break;
    case '$':
        result = parse_time_window();
        break;
    case '#':
        result = parse_tuple_window();
        break;
    default:
        syntax_error("Not a valid unary operator");
    }
    return result;
}

laser::formula::Formula *SimpleRuleReader::parse_diamond() {
    skip_expected_char('D');
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_formula();
    return new laser::formula::Diamond(child);
}

laser::formula::Formula *SimpleRuleReader::parse_box() {
    skip_expected_char('B');
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_formula();
    return new laser::formula::Box(child);
}

laser::formula::Formula *SimpleRuleReader::parse_negation() {
    // TODO
    return new laser::formula::Atom("TODO");
}

laser::formula::Formula *SimpleRuleReader::parse_time_reference() {
    skip_expected_char('@');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_identifier();
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_formula();
    return new laser::formula::TimeReference(argument, child);
}

laser::formula::Formula *SimpleRuleReader::parse_time_window() {
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

laser::formula::Formula *SimpleRuleReader::parse_tuple_window() {
    skip_expected_char('#');
    skip_spaces();
    skip_expected_char(',');
    skip_spaces();
    std::string argument = parse_natural_number();
    skip_spaces();
    skip_expected_char(']');
    auto child = parse_formula();
    uint64_t window_size = std::stoull(argument);
    return new laser::formula::TupleWindow(window_size, child);
}

std::vector<laser::formula::Formula *>
SimpleRuleReader::parse_formula_vector() {
    std::vector<laser::formula::Formula *> result;
    result.push_back(parse_head_atom());
    skip_spaces();
    while (is_next_char_conjunction_operator()) {
        skip_conjunction_operator();
        result.push_back(parse_head_atom());
        skip_spaces();
    }
    return result;
}

laser::formula::Formula *SimpleRuleReader::parse_head_atom() {
    laser::formula::Formula *result;
    if (is_next_char('[')) {
        skip_next_char();
        result = parse_time_reference();
    } else {
        result = parse_predicate_atom();
    }
    return result;
}

void SimpleRuleReader::skip_conjunction_operator() {
    skip_expected_char('&');
    skip_expected_char('&');
}

bool SimpleRuleReader::is_next_char_conjunction_operator() {
    return is_next_char('&');
}

std::vector<laser::rule::Rule> SimpleRuleReader::get_rules() {
    return parse_program();
}
