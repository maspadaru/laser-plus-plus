#include "formula/extended/diamond.h"

namespace laser {
namespace formula {

Diamond::~Diamond() { delete child; }

Formula &Diamond::create() const {
    auto result = new Diamond();
    return *result;
}

Formula &Diamond::clone() const {
    auto result = new Diamond(*this);
    return *result;
}

Formula &Diamond::move() {
    auto result = new Diamond(std::move(*this));
    return *result;
}

// getters / setters

FormulaType Diamond::get_type() const { return FormulaType::DIAMOND; }

std::vector<std::string> Diamond::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Diamond::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Diamond::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Diamond::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Diamond::is_satisfied() const { return child->is_satisfied(); }

bool Diamond::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t Diamond::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Diamond::expire_outdated_groundings(uint64_t current_time,
                                         uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> Diamond::get_groundings() const {
    return child->get_groundings();
}

std::string Diamond::debug_string() const { return child->debug_string(); }

} // namespace formula
} // namespace laser
