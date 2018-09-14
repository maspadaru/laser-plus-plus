#include "formula/extended/implication.h"

namespace laser {
namespace formula {

Implication::~Implication() { delete child; }

Formula &Implication::create() const {
    auto result = new Implication();
    return *result;
}

Formula &Implication::clone() const {
    auto result = new Implication(*this);
    return *result;
}

Formula &Implication::move() {
    auto result = new Implication(std::move(*this));
    return *result;
}

// getters / setters

FormulaType Implication::get_type() const { return FormulaType::IMPLICATION; }

std::vector<std::string> Implication::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Implication::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Implication::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Implication::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Implication::is_satisfied() const { return child->is_satisfied(); }

bool Implication::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t Implication::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Implication::expire_outdated_groundings(uint64_t current_time,
                                             uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> Implication::get_groundings() const {
    return child->get_groundings();
}

std::string Implication::debug_string() const { return child->debug_string(); }

} // namespace formula
} // namespace laser
