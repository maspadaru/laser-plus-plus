#include "formula/extended/box.h"

namespace laser {
namespace formula {

Box::~Box() { delete child; }

Formula &Box::create() const {
    auto result = new Box();
    return *result;
}

Formula &Box::clone() const {
    auto result = new Box(*this);
    return *result;
}

Formula &Box::move() {
    auto result = new Box(std::move(*this));
    return *result;
}

// getters / setters

FormulaType Box::get_type() const { return FormulaType::BOX; }

std::vector<std::string> Box::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Box::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Box::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Box::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Box::is_satisfied() const { return child->is_satisfied(); }

bool Box::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t Box::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Box::expire_outdated_groundings(uint64_t current_time,
                                     uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> Box::get_groundings() const {
    return child->get_groundings();
}

std::string Box::debug_string() const { return child->debug_string(); }

} // namespace formula
} // namespace laser
