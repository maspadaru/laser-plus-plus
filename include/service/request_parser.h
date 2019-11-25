#ifndef LASER_SERVICE_REQUEST_PARSER_H
#define LASER_SERVICE_REQUEST_PARSER_H

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "formula/extended/atom.h"
#include "formula/formula.h"
#include "service/query.h"
#include "service/request.h"
#include "util/format_exception.h"

namespace laser {
namespace service {

class RequestParser {
  private:
    inline void syntax_error(std::string const &error_message) const;

    void skip_spaces();
    void skip_next_char();
    char read_next_char();
    void skip_expected_char(char c);
    void skip_conjunction_operator();
    char peek_next_char();
    bool is_next_char_letter();
    bool is_next_char_digit();
    bool is_next_char(char c);
    bool is_next_char_letter_or_digit();
    bool is_next_char_binary_operator();
    bool is_next_char_conjunction_operator();
    bool is_next_char_math_operator();
    bool is_next_char_comparison_operator();
    void parse_eoln();
    std::string parse_math_sign();
    std::string parse_comparison_sign();
    char parse_nonzero();
    std::string parse_zero();
    char parse_diggit();
    char parse_letter();
    std::string parse_positive_number();
    std::string parse_natural_number();
    std::string parse_integer();
    std::string parse_float();
    std::string parse_identifier();
    std::string parse_argument();
    std::string parse_float_argument();

    std::unique_ptr<laser::formula::Formula> parse_predicate_atom();

    // ================

    std::vector<std::shared_ptr<util::Grounding>> const &database_facts;

    std::set<std::string> parse_variable_list();
    std::vector<std::unique_ptr<laser::formula::Formula>> parse_atom_vector();
    std::stringstream input;
    std::unique_ptr<Request> parse_query();

    std::vector<size_t> compute_known_index_vector(
        std::unique_ptr<formula::Formula> const &query_formula,
        std::set<std::string> const &unknown_variables) const;

    util::Grounding compute_trigger(
        std::unique_ptr<formula::Formula> const &query_formula) const;

  public:
    explicit RequestParser(
        std::vector<std::shared_ptr<util::Grounding>> const &database_facts);

    ~RequestParser() = default;

    std::unique_ptr<Request> parse_request(std::string const &request_string);
};

} // namespace service
} // namespace laser

#endif // LASER_SERVICE_REQUEST_PARSER_H
