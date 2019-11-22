#include "formula/extended/conjunction.h"

#include <algorithm>

namespace laser::formula {

Conjunction::Conjunction(std::unique_ptr<formula::Formula> left_child,
                         std::unique_ptr<formula::Formula> right_child,
                         bool is_head)
    : left_child(std::move(left_child)), right_child(std::move(right_child)) {
    is_head_m = is_head;
    populate_variable_collections();
    compute_predicate_vector();
    compute_arity_map();
}

Conjunction::Conjunction(std::unique_ptr<formula::Formula> left_child,
                         std::unique_ptr<formula::Formula> right_child)
    : left_child(std::move(left_child)), right_child(std::move(right_child)) {
    populate_variable_collections();
    compute_predicate_vector();
    compute_arity_map();
}

std::unique_ptr<formula::Formula> Conjunction::clone() const {
    auto new_left = left_child->clone();
    auto new_right = right_child->clone();
    return std::make_unique<formula::Conjunction>(
        std::move(new_left), std::move(new_right), is_head_m);
}

bool Conjunction::is_head() const { return is_head_m; }

void Conjunction::set_head(bool is_head) {
    // Conjunctino in head is used for Existentialy quantified rules using
    // restricted chase algorithm.
    // Intentionaly not setting is_head in children. We want ExactTime atoms
    // to evaluate as if they were in the body.
    is_head_m = is_head;
}

void Conjunction::populate_variable_collections() {
    // Get all variables from children and insert into variable_names
    auto left = left_child->get_variable_names();
    left.erase(unique(left.begin(), left.end()), left.end());
    auto right = right_child->get_variable_names();
    right.erase(unique(right.begin(), right.end()), right.end());
    for (auto const &left_var : left) {
        for (auto const &right_var : right) {
        }
    }
    variable_names.insert(variable_names.end(),
                          std::make_move_iterator(left.begin()),
                          std::make_move_iterator(left.end()));
    variable_names.insert(variable_names.end(),
                          std::make_move_iterator(right.begin()),
                          std::make_move_iterator(right.end()));
    sort(variable_names.begin(), variable_names.end());
    // Get all duplicates, and insert into common_child variables
    for (size_t i = 0; i < variable_names.size() - 1; i++) {
        if (variable_names[i] == variable_names[i + 1]) {
            common_child_variables.push_back(variable_names[i]);
        }
    }
    // Remove all duplicates (variables found in both children)
    variable_names.erase(unique(variable_names.begin(), variable_names.end()),
                         variable_names.end());
    // populate variable_map
    for (size_t i = 0; i < variable_names.size(); i++) {
        variable_map.try_emplace(variable_names[i], i);
    }
}

FormulaType Conjunction::get_type() const { return FormulaType::CONJUNCTION; }

std::vector<std::string> const &Conjunction::get_predicate_vector() const {
    return predicate_vector;
}

void Conjunction::compute_predicate_vector() {
    auto left = left_child->get_predicate_vector();
    auto right = right_child->get_predicate_vector();
    concatenate_vectors(left, right);
}

std::map<std::string, size_t> const &Conjunction::get_arity_map() const {
    return arity_map;
}

void Conjunction::compute_arity_map() {
    arity_map = left_child->get_arity_map();
    auto right_arity_map = right_child->get_arity_map();
    arity_map.insert(right_arity_map.begin(), right_arity_map.end());
}

void Conjunction::concatenate_vectors(std::vector<std::string> const &left,
                                      std::vector<std::string> const &right) {
    predicate_vector = left;
    predicate_vector.insert(predicate_vector.end(), right.begin(), right.end());
    sort(predicate_vector.begin(), predicate_vector.end());
    predicate_vector.erase(
        unique(predicate_vector.begin(), predicate_vector.end()),
        predicate_vector.end());
}

std::vector<std::string> const &Conjunction::get_variable_names() const {
    return variable_names;
}

int Conjunction::get_variable_index(std::string const &variable_name) const {
    int result = -1;
    if (variable_map.count(variable_name) > 0) {
        result = variable_map.at(variable_name);
    }
    return result;
}

size_t Conjunction::get_number_of_variables() const {
    return variable_names.size();
}

void Conjunction::add_child(std::unique_ptr<formula::Formula> child) {
    if (!left_child) {
        left_child = std::move(child);
    } else if (!right_child) {
        right_child = std::move(child);
        populate_variable_collections();
        compute_predicate_vector();
    }
}

void Conjunction::expire_outdated_groundings(util::Timeline const &timeline) {
    left_child->expire_outdated_groundings(timeline);
    right_child->expire_outdated_groundings(timeline);
    grounding_set.clear();
}

std::vector<std::shared_ptr<util::Grounding>>
Conjunction::get_groundings(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result_vector;
    std::copy(grounding_set.begin(), grounding_set.end(),
              std::back_inserter(result_vector));
    return result_vector;
}

std::vector<std::shared_ptr<util::Grounding>>
Conjunction::get_conclusions_timepoint(util::Timeline const &timeline) {
    return get_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Conjunction::get_conclusions_step(util::Timeline const &timeline) {
    return get_groundings(timeline);
}

std::string
Conjunction::hash_common_variables(Formula const &child,
                                   util::Grounding const &grounding) const {
    if (common_child_variables.empty()) {
        return "0";
    }
    std::vector<std::string> vector;
    for (auto &variable : common_child_variables) {
        size_t variable_index = child.get_variable_index(variable);
        auto const &constant = grounding.get_constant(variable_index);
        vector.push_back(constant);
    }
    std::string result =
        std::accumulate(vector.begin(), vector.end(), std::string(";"));
    return result;
}

std::shared_ptr<util::Grounding>
Conjunction::merge_groundings(util::Timeline const &timeline,
                              util::Grounding const &left,
                              util::Grounding const &right) const {
    auto ct = timeline.get_time();
    auto ht = timeline.min(left.get_horizon_time(), right.get_horizon_time());
    // TODO not sure about tuple counter values;
    auto cc = timeline.max(left.get_horizon_count(), right.get_horizon_count());
    auto hc = timeline.min(left.get_horizon_count(), right.get_horizon_count());
    std::vector<std::string> substitution_vector;
    for (auto const &variable_name : variable_names) {
        auto left_index = left_child->get_variable_index(variable_name);
        if (left_index >= 0) {
            substitution_vector.push_back(left.get_constant(left_index));
        } else {
            auto right_index = right_child->get_variable_index(variable_name);
            substitution_vector.push_back(right.get_constant(right_index));
        }
    }
    auto result = std::make_shared<util::Grounding>("", ct, ht, cc, hc,
                                                    substitution_vector);
    auto max_step = left.get_step();
    if (max_step < right.get_step()) {
        max_step = right.get_step();
    }
    result->set_step(max_step);
    return result;
}

void Conjunction::populate_grounding_set(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &left_groundings,
    std::vector<std::shared_ptr<util::Grounding>> const &right_groundings) {
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<util::Grounding>>>
        hashmap;
    auto current_time = timeline.get_time();
    for (auto const &gr : right_groundings) {
        auto key = hash_common_variables(*right_child, *gr);
        hashmap.try_emplace(key);
        std::vector<std::shared_ptr<util::Grounding>> &map_vector =
            hashmap[key];
        map_vector.push_back(gr);
    }
    for (auto const &gl : left_groundings) {
        auto gl_fresh = gl->is_fresh_sne(current_time, previous_step);
        auto key = hash_common_variables(*left_child, *gl);
        if (hashmap.count(key) > 0) {
            auto hashmap_values = hashmap.at(key);
            for (auto const &gr : hashmap_values) {
                auto gr_fresh = gr->is_fresh_sne(current_time, previous_step);
                if (gl_fresh || gr_fresh) {
                    auto new_grounding = merge_groundings(timeline, *gl, *gr);
                    grounding_set.insert(std::move(new_grounding));
                }
            }
        }
    }
}

bool Conjunction::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    // TODO Here I can split facts in sub-vectors, based on predicates of
    // children
    left_child->evaluate(timeline, previous_step, facts);
    right_child->evaluate(timeline, previous_step, facts);
    auto left_groundings = left_child->get_groundings(timeline);
    auto right_groundings = right_child->get_groundings(timeline);
    populate_grounding_set(timeline, previous_step, left_groundings,
                           right_groundings);
    return !grounding_set.empty();
}

std::vector<std::unique_ptr<formula::Formula> const *>
Conjunction::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    result.push_back(&left_child);
    result.push_back(&right_child);
    return result;
}

uint64_t Conjunction::get_window_size() const { return 0; }

} // namespace laser::formula
