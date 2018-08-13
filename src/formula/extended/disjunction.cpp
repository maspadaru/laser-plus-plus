#include "formula/extended/disjunction.h"

namespace laser {
namespace formula {


Disjunction::~Disjunction() {
    delete child;
}

Formula &Disjunction::create() const {
    auto result = new Disjunction();
    return *result;
}

Formula &Disjunction::clone() const {
    auto result = new Disjunction(*this);
    return *result;
}

Formula &Disjunction::move() {
   auto result = new Disjunction(std::move(*this));
   return *result;
}

// getters / setters

FormulaType Disjunction::get_type() const {
    return FormulaType::DISJUNCTION; 
}

std::vector<std::string> Disjunction::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Disjunction::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Disjunction::get_full_variable_names() const {
    return child ->get_full_variable_names();
}

int Disjunction::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Disjunction::is_satisfied() const {
    return child->is_satisfied();
}

bool Disjunction::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t Disjunction::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Disjunction::expire_outdated_groundings(uint64_t current_time,
                                        uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> Disjunction::get_groundings() const {
    return child->get_groundings();
}

std::string Disjunction::debug_string() const {
    return child->debug_string();
}


} // namespace formula
} // namespace laser
