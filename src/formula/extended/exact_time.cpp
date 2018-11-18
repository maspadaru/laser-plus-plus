#include "formula/extended/exact_time.h"

namespace laser {
namespace formula {

ExactTime::~ExactTime() { delete child; }

ExactTime::ExactTime(Formula *child) { this->child = &child->move(); }

Formula &ExactTime::create() const {
    auto result = new ExactTime();
    return *result;
}

Formula &ExactTime::clone() const {
    auto result = new ExactTime(&this->child->clone());
    return *result;
}

Formula &ExactTime::move() {
    auto result = new ExactTime(&this->child->move());
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

size_t ExactTime::get_number_of_variables() const {
    return child->get_number_of_variables();
}

std::string ExactTime::debug_string() const { return child->debug_string(); }

void ExactTime::add_child(formula::Formula *child) {}

std::vector<Grounding> ExactTime::get_groundings(util::Timeline timeline) {
     auto grounding_vector = grounding_table.get_all_groundings();
     for (auto &grounding : grounding_vector) {
         grounding.set_horizon_time(util::Timeline::INFINITE_TIME);
     } 
     return grounding_vector;
}

std::vector<Grounding> ExactTime::get_conclusions(util::Timeline timeline) {
    return grounding_table.get_recent_groundings();
}

bool ExactTime::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    bool result = child->evaluate(timeline, facts);
    auto child_facts = child->get_groundings(timeline);
    grounding_table.add_grounding_vector(child_facts);
    return grounding_table.has_recent_groundings();
}

void ExactTime::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
    grounding_table.expire_outdated_groundings(timeline.get_min_time(),
                                               timeline.get_min_tuple_count());
}


} // namespace formula
} // namespace laser
