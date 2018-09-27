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
    // TODO
    return *result;
}

Formula &Diamond::move() {
    auto result = new Diamond(std::move(*this));
    // TODO
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

size_t Diamond::get_number_of_variables() const {
    return child->get_number_of_variables();
}

std::vector<Grounding> Diamond::get_groundings() const {
    return child->get_groundings();
}

std::string Diamond::debug_string() const { return child->debug_string(); }

bool Diamond::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    auto predicate_vector = child->get_predicate_vector();
    for (auto &predicate : predicate_vector) {
        auto &fact_vector = facts[predicate];
        for (auto &fact: fact_vector) {
            if (fact.get_horizon_time() == timeline.get_time()) {
                fact.set_horizon_time(timeline.get_max_time());
            }
        }
    }
    return child->evaluate(timeline, facts);
}

void Diamond::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
}

void Diamond::add_child(formula::Formula* child) {} 
Diamond::Diamond(Formula* child) {
    this->child = &child->move();
}


} // namespace formula
} // namespace laser
