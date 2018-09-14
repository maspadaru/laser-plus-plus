#include "formula/extended/exact_time.h"

namespace laser {
namespace formula {

ExactTime::~ExactTime() { delete child; }

Formula &ExactTime::create() const {
    auto result = new ExactTime();
    return *result;
}

Formula &ExactTime::clone() const {
    auto result = new ExactTime(*this);
    return *result;
}

Formula &ExactTime::move() {
    auto result = new ExactTime(std::move(*this));
    return *result;
}

// getters / setters

FormulaType ExactTime::get_type() const { return FormulaType::EXACT_TIME; }

std::vector<std::string> ExactTime::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> ExactTime::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> ExactTime::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int ExactTime::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool ExactTime::is_satisfied() const { return child->is_satisfied(); }

bool ExactTime::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t ExactTime::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void ExactTime::expire_outdated_groundings(uint64_t current_time,
                                           uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> ExactTime::get_groundings() const {
    return child->get_groundings();
}

std::string ExactTime::debug_string() const { return child->debug_string(); }

} // namespace formula
} // namespace laser
