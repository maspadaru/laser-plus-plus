#include "formula/extended/diamond.h"

namespace laser {
namespace formula {

Diamond::~Diamond() { delete child; }

Diamond::Diamond(Formula *child) { this->child = &child->move(); }

Formula &Diamond::create() const {
    auto result = new Diamond();
    return *result;
}

Formula &Diamond::clone() const {
    auto result = new Diamond(&this->child->clone());
    return *result;
}

Formula &Diamond::move() {
    auto result = new Diamond(&this->child->move());
    return *result;
}

// getters / setters

void Diamond::set_head(bool is_head) {
    is_head_m = is_head;
}

bool Diamond::is_head() const {
    return is_head_m;
}

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

std::string Diamond::debug_string() const { return child->debug_string(); }

void Diamond::add_child(formula::Formula *child) {}

std::vector<Grounding> Diamond::get_groundings(util::Timeline timeline) {
     auto grounding_vector = grounding_table.get_all_groundings();
     for (auto &grounding : grounding_vector) {
         grounding.set_horizon_time(util::Timeline::INFINITE_TIME);
     } 
     return grounding_vector;
}

std::vector<Grounding> Diamond::get_conclusions_timepoint(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::vector<Grounding> Diamond::get_conclusions_step(util::Timeline timeline) {
    return grounding_table.get_recent_groundings();
}

bool Diamond::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    bool result = child->evaluate(timeline, facts);
    auto child_facts = child->get_groundings(timeline);
    grounding_table.add_grounding_vector(child_facts);
    return result;
}

void Diamond::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
    grounding_table.expire_outdated_groundings(timeline.get_min_time(),
                                               timeline.get_min_tuple_count());
}


} // namespace formula
} // namespace laser
