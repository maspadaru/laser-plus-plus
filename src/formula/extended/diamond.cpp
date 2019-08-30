#include "formula/extended/diamond.h"

namespace laser::formula {

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

void Diamond::set_head(bool is_head) { is_head_m = is_head; }

bool Diamond::is_head() const { return is_head_m; }

FormulaType Diamond::get_type() const { return FormulaType::DIAMOND; }

std::vector<std::string> const &Diamond::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::map<std::string, size_t> const &Diamond::get_arity_map() const {
    return child->get_arity_map();
}

std::vector<std::string> const &Diamond::get_variable_names() const {
    return child->get_variable_names();
}

int Diamond::get_variable_index(std::string const &variable_name) const {
    return child->get_variable_index(variable_name);
}

size_t Diamond::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Diamond::add_child(formula::Formula *child) {}

std::vector<std::shared_ptr<util::Grounding>>
Diamond::get_groundings(util::Timeline const &timeline) {
    auto const &grounding_vector = grounding_table.get_all_groundings();
    auto current_time = timeline.get_time();
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &grounding : grounding_vector) {
        auto new_grounding = grounding->clone();
        auto const &predicate = new_grounding->get_predicate();
        // Answer stream interpr of minimal model, therefore diamond only 
        // consideres facts in the data stream, not answer stream
        // ref: LARS peper, Definition 9.
        if ( extensional_predicates.count(predicate) > 0 ) {
            new_grounding->set_horizon_time(util::Timeline::INFINITE_TIME);
        } else {
            new_grounding->set_horizon_time(current_time);
        }
        result.push_back(std::move(new_grounding));
    }
    return result;
}

void Diamond::set_extensional_predicates(
    std::set<std::string> const &extensional_predicates) {
    this->extensional_predicates = extensional_predicates;
    child->set_extensional_predicates(extensional_predicates);
}

std::vector<std::shared_ptr<util::Grounding>>
Diamond::get_conclusions_timepoint(util::Timeline const &timeline) {
    return get_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Diamond::get_conclusions_step(util::Timeline const &timeline) {
    return grounding_table.get_recent_groundings();
}

bool Diamond::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    bool result = child->evaluate(timeline, previous_step, facts);
    auto child_facts = child->get_groundings(timeline);
    grounding_table.add_grounding_vector(child_facts);
    return result;
}

void Diamond::expire_outdated_groundings(util::Timeline const &timeline) {
    child->expire_outdated_groundings(timeline);
    auto time = timeline.get_min_time();
    auto tuple_count = timeline.get_tuple_count_at(timeline.get_time());
    grounding_table.expire_outdated_groundings(time, tuple_count);
}

std::vector<formula::Formula *> Diamond::get_children() const {
    std::vector<formula::Formula *> result;
    result.push_back(child);
    return result;
}

uint64_t Diamond::get_window_size() const { return 0; }

} // namespace laser::formula
