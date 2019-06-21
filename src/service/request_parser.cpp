#include "service/request_parser.h"

namespace laser {
namespace service {

inline void
RequestParser::syntax_error(std::string const &error_message) const {
    std::string message = "Syntax Error: " + error_message;
    const char *exception_message = message.c_str();
    throw util::FormatException(exception_message);
}

void RequestParser::skip_spaces() {
    input >> std::ws; // eats-up any leading whitespaces.
}

void RequestParser::skip_next_char() { input.get(); }
char RequestParser::read_next_char() { return input.get(); }

void RequestParser::skip_expected_char(char c) {
    char input_char = read_next_char();
    if (input_char != c) {
        std::stringstream error_stream;
        error_stream << "Expected character [" << c
                     << "] but actual caracter was [" << input_char << "].";
        std::string error_message = error_stream.str();
        syntax_error(error_message);
    }
}

void RequestParser::skip_conjunction_operator() {
    skip_expected_char('&');
    skip_expected_char('&');
}

char RequestParser::peek_next_char() { return input.peek(); }

bool RequestParser::is_next_char_letter() {
    return std::isalpha(peek_next_char()) > 0;
}

bool RequestParser::is_next_char_digit() {
    return std::isdigit(peek_next_char()) > 0;
}

bool RequestParser::is_next_char_letter_or_digit() {
    return is_next_char_letter() || is_next_char_digit();
}

bool RequestParser::is_next_char(char c) { return peek_next_char() == c; }

bool RequestParser::is_next_char_binary_operator() {
    return is_next_char('&') || is_next_char('|');
}

bool RequestParser::is_next_char_conjunction_operator() {
    return is_next_char('&');
}

bool RequestParser::is_next_char_math_operator() {
    return is_next_char('+') || is_next_char('-') || is_next_char('*') ||
           is_next_char('/');
}

bool RequestParser::is_next_char_comparison_operator() {
    return is_next_char('<') || is_next_char('>') || is_next_char('=') ||
           is_next_char('!');
}

void RequestParser::parse_eoln() {
    skip_spaces();
    if (!input.eof()) {
        syntax_error("Expecting EOL");
    }
}

std::string RequestParser::parse_math_sign() {
    // TODO
    return "";
}

std::string RequestParser::parse_comparison_sign() {
    // TODO
    return "";
}

char RequestParser::parse_nonzero() {
    char result = read_next_char();
    if (std::isdigit(result) == 0 || result == '0') {
        syntax_error("Expected non-zero digit");
    }
    return result;
}

std::string RequestParser::parse_zero() {
    skip_next_char();
    return "0";
}

char RequestParser::parse_diggit() {
    char result = read_next_char();
    if (std::isdigit(result) == 0) {
        syntax_error("Expected digit");
    }
    return result;
}

char RequestParser::parse_letter() {
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

std::string RequestParser::parse_positive_number() {
    std::stringstream result_stream;
    result_stream << parse_nonzero();
    while (is_next_char_digit()) {
        result_stream << read_next_char();
    }
    return result_stream.str();
}

std::string RequestParser::parse_natural_number() {
    if (is_next_char('0')) {
        return parse_zero();
    }
    return parse_positive_number();
}

std::string RequestParser::parse_integer() {
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

std::string RequestParser::parse_float() {
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

std::string RequestParser::parse_identifier() {
    std::stringstream result_stream;
    result_stream << parse_letter();
    while (is_next_char_letter_or_digit()) {
        result_stream << read_next_char();
    }
    return result_stream.str();
}

std::string RequestParser::parse_argument() {
    if (is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_integer();
}

std::string RequestParser::parse_float_argument() {
    if (is_next_char_letter()) {
        return parse_identifier();
    }
    return parse_float();
}

laser::formula::Formula *RequestParser::parse_predicate_atom() {
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

//==========================================

RequestParser::RequestParser(
    std::vector<std::shared_ptr<util::Grounding>> const &db_facts)
    : database_facts(db_facts) {}

std::vector<laser::formula::Formula *> RequestParser::parse_atom_vector() {
    std::vector<laser::formula::Formula *> result;
    result.push_back(parse_predicate_atom());
    skip_spaces();
    while (is_next_char_conjunction_operator()) {
        skip_conjunction_operator();
        result.push_back(parse_predicate_atom());
        skip_spaces();
    }
    return result;
}

std::unique_ptr<Request>
RequestParser::parse_request(std::string const &request_string) {
    std::unique_ptr<Request> result;
    input.str(request_string);
    auto head = read_next_char();
    if (head == 'Q') {
        skip_spaces();
        skip_expected_char(':');
        skip_spaces();
        result = parse_query();
    }
    return result;
}

std::set<std::string> RequestParser::parse_variable_list() {
    std::set<std::string> result;
    std::string argument = parse_identifier();
    result.insert(argument);
    skip_spaces();
    while (is_next_char(',')) {
        skip_next_char();
        skip_spaces();
        std::string argument = parse_identifier();
        result.insert(argument);
        skip_spaces();
    }
    return result;
}

std::vector<size_t> RequestParser::compute_known_index_vector(
    formula::Formula *query_formula,
    std::set<std::string> const &unknown_variables) const {
    std::vector<size_t> known_index_vector;
    auto all_variables = query_formula->get_variable_names();
    for (auto const &var_name : all_variables) {
        if (unknown_variables.count(var_name) == 0) {
            auto index = query_formula->get_variable_index(var_name);
            known_index_vector.push_back(index);
        }
    }
    return known_index_vector;
}

util::Grounding
RequestParser::compute_trigger(formula::Formula *query_formula) const {
    auto predicate = query_formula->get_predicate_vector().at(0);
    auto constant_vector = query_formula->get_variable_names();
    auto result = util::Grounding(predicate, 1, 1, 1, 1, constant_vector);
    return result;
}

std::unique_ptr<Request> RequestParser::parse_query() {
    std::unique_ptr<Request> result;
    auto unknown_variables = parse_variable_list();
    skip_spaces();
    skip_expected_char('?');
    skip_spaces();
    auto query_formula = parse_atom_vector();
    if (query_formula.size() == 1) {
        auto query_atom = query_formula.at(0);
        auto known_index_vector =
            compute_known_index_vector(query_atom, unknown_variables);
        auto trigger = compute_trigger(query_atom);
        // Query query (query_atom, trigger, known_index_vector,
        // database_facts);
        result = std::make_unique<Query>(query_atom, trigger,
                                         known_index_vector, database_facts);
    } else {
        // TODO conunctive query
    }
    return result;
}

} // namespace service
} // namespace laser
