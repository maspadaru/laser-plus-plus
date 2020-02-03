#include "formula/extended/diamond.h"

namespace laser::formula {

Diamond::Diamond(std::unique_ptr<formula::Formula> child)
    : child(std::move(child)) {}

std::unique_ptr<formula::Formula> Diamond::clone() const {
    auto child_clone = child->clone();
    return std::make_unique<formula::Diamond>(std::move(child_clone));
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

void Diamond::add_child(std::unique_ptr<formula::Formula> child) {}

std::vector<std::shared_ptr<util::Grounding>> Diamond::convert_facts(
    std::vector<std::shared_ptr<util::Grounding>> const &grounding_vector,
    util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &grounding : grounding_vector) {
        auto new_grounding = grounding->shallow_clone();
        new_grounding->set_horizon_time(util::Timeline::INFINITE_TIME);
        result.push_back(std::move(new_grounding));
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Diamond::get_new_facts(util::Timeline const &timeline) {
    auto const &grounding_vector = grounding_table.get_new_groundings();
    return convert_facts(grounding_vector, timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Diamond::get_old_facts(util::Timeline const &timeline) {
    auto const &grounding_vector = grounding_table.get_old_groundings();
    return convert_facts(grounding_vector, timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Diamond::get_conclusions(util::Timeline const &timeline) {
    auto time = timeline.get_time();
    auto result = grounding_table.get_groundings_at(time);
    auto new_facts = grounding_table.get_new_groundings();
    result.insert(result.end(), std::make_move_iterator(new_facts.begin()),
                  std::make_move_iterator(new_facts.end()));
    return result;
}

void Diamond::new_step(uint64_t current_time) {
    grounding_table.new_step(current_time);
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

std::vector<std::unique_ptr<formula::Formula> const *>
Diamond::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    result.push_back(&child);
    return result;
}

uint64_t Diamond::get_window_size() const { return 0; }

} // namespace laser::formula
