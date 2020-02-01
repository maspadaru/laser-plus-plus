#include "formula/extended/time_reference.h"

namespace laser::formula {

TimeReference::TimeReference(std::string time_variable,
                             std::unique_ptr<formula::Formula> child)
    : time_variable(std::move(time_variable)), child(std::move(child)) {
    init();
}

std::unique_ptr<formula::Formula> TimeReference::clone() const {
    auto child_clone = child->clone();
    return std::make_unique<formula::TimeReference>(time_variable,
                                                    std::move(child_clone));
}

void TimeReference::init() {
    auto variable_names = child->get_variable_names();
    // Add time_variable on the last index of the vector
    variable_names.push_back(time_variable);
    grounding_table.set_variable_names(variable_names);
}

void TimeReference::set_head(bool is_head) { child->set_head(is_head); }

bool TimeReference::is_head() const { return child->is_head(); }

size_t TimeReference::get_time_variable_index() const {
    return grounding_table.get_variable_index(time_variable);
}

FormulaType TimeReference::get_type() const { return formula_type; }

std::vector<std::string> const &TimeReference::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::map<std::string, size_t> const &TimeReference::get_arity_map() const {
    return child->get_arity_map();
}

std::vector<std::string> const &TimeReference::get_variable_names() const {
    return grounding_table.get_variable_names();
}

int TimeReference::get_variable_index(std::string const &variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

size_t TimeReference::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

void TimeReference::add_child(std::unique_ptr<formula::Formula> child) {}

std::shared_ptr<util::Grounding>
TimeReference::add_time_variable(util::Timeline const &timeline,
                                 util::Grounding const &grounding) const {
    auto result = grounding.deep_clone();
    result->set_constant(get_time_variable_index(),
                         std::move(std::to_string(timeline.get_time())));
    return result;
}

std::shared_ptr<util::Grounding>
TimeReference::remove_time_variable(util::Grounding const &grounding) const {
    auto result = grounding.deep_clone();
    result->remove_constant(get_time_variable_index());
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
TimeReference::convert_groundings_head(
    std::vector<std::shared_ptr<util::Grounding>> const &groundings) const {
    std::vector<std::shared_ptr<util::Grounding>> result_vector;
    for (auto const &grounding : groundings) {
        size_t timevar_index =
            grounding_table.get_variable_index(time_variable);
        std::string const &horizon_time_string =
            grounding->get_constant(timevar_index);
        uint64_t horizon_time = std::stoull(horizon_time_string);
        auto new_grounding = grounding->shallow_clone();
        new_grounding->set_horizon_time(horizon_time);
        result_vector.push_back(std::move(new_grounding));
    }
    return result_vector;
}

std::vector<std::shared_ptr<util::Grounding>>
TimeReference::convert_groundings_body(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> groundings) const {
    std::vector<std::shared_ptr<util::Grounding>> result_vector;
    for (auto const &grounding : groundings) {
        auto new_grounding = add_time_variable(timeline, *grounding);
        result_vector.push_back(std::move(new_grounding));
    }
    return result_vector;
}

std::vector<std::shared_ptr<util::Grounding>> TimeReference::revert_groundings(
    uint64_t now,
    std::vector<std::shared_ptr<util::Grounding>> groundings) const {
    std::vector<std::shared_ptr<util::Grounding>> result_vector;
    for (auto const &grounding : groundings) {
        auto new_grounding = remove_time_variable(*grounding);
        new_grounding->set_consideration_time(now);
        result_vector.push_back(std::move(new_grounding));
    }
    return result_vector;
}

std::vector<std::shared_ptr<util::Grounding>>
TimeReference::get_conclusions(util::Timeline const &timeline) {
    get_new_facts(timeline); // just to populate timepoint_conclusions
    return timepoint_conclusions;
}

std::vector<std::shared_ptr<util::Grounding>>
TupleReference::get_old_facts(util::Timeline const &timeline) {
    auto grounding_vector = grounding_table.get_old_groundings();
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto &grounding : grounding_vector) {
        std::string const &timevar_string =
            grounding->get_constant(get_time_variable_index());
        uint64_t timevar_value = std::stoull(timevar_string);
        if (timevar_value <= timeline.get_time()) {
            result.push_back(std::move(grounding));
        }
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
TimeReference::get_new_facts(util::Timeline const &timeline) {
    // 1. Get recent groundings and add them to conclusions_vector or
    // furure_conclusion_map
    auto now = timeline.get_time();
    std::vector<std::shared_ptr<util::Grounding>> conclusions_vector;
    auto grounding_vector =
        grounding_table.get_new_groundings();
    for (auto &grounding : grounding_vector) {
        std::string const &timevar_string =
            grounding->get_constant(get_time_variable_index());
        uint64_t timevar_value = std::stoull(timevar_string);
        if (timevar_value == now) {
            conclusions_vector.push_back(std::move(grounding));
        } else {
            future_conclusion_map.try_emplace(timevar_value);
            std::set<std::shared_ptr<util::Grounding>,
                     util::GroundingFullCompare> &map_set =
                future_conclusion_map[timevar_value];
            map_set.insert(std::move(grounding));
        }
    }
    // 2. Add all groundings from future_conclusion_map to conclusions_vector
    std::set<std::shared_ptr<util::Grounding>, util::GroundingFullCompare>
        &grounding_set = future_conclusion_map[now];
    std::move(grounding_set.begin(), grounding_set.end(),
              std::back_inserter(conclusions_vector));
    future_conclusion_map.erase(now);
    // 3. Conclusions should not contain the time variable
    auto result = revert_groundings(now, std::move(conclusions_vector));
    // 4. Copy result into timepoint_conclusions before returning
    std::copy(result.begin(), result.end(),
              std::back_inserter(timepoint_conclusions));
    return result;
}

void TimeReference::evaluate_head(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    std::string predicate = get_predicate_vector().at(0);
    auto exact_time_groundings = convert_groundings_head(facts);
    grounding_table.add_grounding_vector(exact_time_groundings);
}

void TimeReference::evaluate_body(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    child->evaluate(timeline, previous_step, facts);
    auto child_conclusions = child->get_groundings(timeline);
    auto exact_time_groundings =
        convert_groundings_body(timeline, child_conclusions);
    grounding_table.add_grounding_vector(exact_time_groundings);
}

bool TimeReference::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    // If the formula is in the head of the rule, we know the child can only
    // be an Atom. So we can ignore the child.
    if (is_head()) {
        evaluate_head(timeline, previous_step, facts);
    } else {
        evaluate_body(timeline, previous_step, facts);
    }
    return grounding_table.has_recent_groundings();
}

void TimeReference::new_step(uint64_t current_time) {
    child->new_step(current_time);
}

void TimeReference::expire_outdated_groundings(util::Timeline const &timeline) {
    timepoint_conclusions.clear();
    child->expire_outdated_groundings(timeline);
    auto time = timeline.get_min_time();
    auto tuple_count = timeline.get_tuple_count_at(timeline.get_time());
    grounding_table.expire_outdated_groundings(time, tuple_count);
}

std::vector<std::unique_ptr<formula::Formula> const *>
TimeReference::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    result.push_back(&child);
    return result;
}

uint64_t TimeReference::get_window_size() const { return 0; }

} // namespace laser::formula
