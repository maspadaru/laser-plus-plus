#include "formula/extended/conjunction.h"

namespace laser {
namespace formula {

Conjunction::~Conjunction() { delete child; }

Formula &Conjunction::create() const {
    auto result = new Conjunction();
    return *result;
}

Formula &Conjunction::clone() const {
    auto result = new Conjunction(*this);
    return *result;
}

Formula &Conjunction::move() {
    auto result = new Conjunction(std::move(*this));
    return *result;
}

// getters / setters

FormulaType Conjunction::get_type() const { return FormulaType::CONJUNCTION; }

std::vector<std::string> Conjunction::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Conjunction::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Conjunction::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Conjunction::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Conjunction::is_satisfied() const { return child->is_satisfied(); }

bool Conjunction::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t Conjunction::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Conjunction::expire_outdated_groundings(uint64_t current_time,
                                             uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> Conjunction::get_groundings() const {
    return child->get_groundings();
}

std::string Conjunction::debug_string() const { return child->debug_string(); }

} // namespace formula
} // namespace laser
