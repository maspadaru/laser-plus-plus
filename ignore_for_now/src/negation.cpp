#include "formula/extended/negation.h"

namespace laser {
namespace formula {


Negation::~Negation() {
    delete child;
}

Formula &Negation::create() const {
    auto result = new Negation();
    return *result;
}

Formula &Negation::clone() const {
    auto result = new Negation(*this);
    return *result;
}

Formula &Negation::move() {
   auto result = new Negation(std::move(*this));
   return *result;
}

// getters / setters

FormulaType Negation::get_type() const {
    return FormulaType::NEGATION; 
}

std::vector<std::string> Negation::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Negation::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Negation::get_full_variable_names() const {
    return child ->get_full_variable_names();
}

int Negation::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Negation::is_satisfied() const {
    return child->is_satisfied();
}

bool Negation::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t Negation::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Negation::expire_outdated_groundings(uint64_t current_time,
                                        uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> Negation::get_groundings() const {
    return child->get_groundings();
}

std::string Negation::debug_string() const {
    return child->debug_string();
}


} // namespace formula
} // namespace laser
