#include "rule/filter/indexed_filter.h"

namespace laser::rule {

ChaseFilter *IndexedFilter::create() const {
    auto result = new IndexedFilter();
    return result;
}

IndexedFilter::~IndexedFilter() { delete head_formula; }

ChaseFilter *IndexedFilter::clone() const {
    auto result = new IndexedFilter();
    result->frontier_variables = this->frontier_variables;
    result->head_variables = this->head_variables;
    result->free_variables = this->free_variables;
    result->bound_variables = this->bound_variables;
    result->free_variable_index = this->free_variable_index;
    result->bound_variable_index = this->bound_variable_index;
    result->null_value_count = this->null_value_count;
    result->head_formula = &this->head_formula->clone();
    return result;
}

ChaseFilter *IndexedFilter::move() {
    auto result = new IndexedFilter();
    result->frontier_variables = std::move(this->frontier_variables);
    result->head_variables = std::move(this->head_variables);
    result->free_variables = std::move(this->free_variables);
    result->bound_variables = std::move(this->bound_variables);
    result->free_variable_index = std::move(this->free_variable_index);
    result->bound_variable_index = std::move(this->bound_variable_index);
    result->null_value_count = this->null_value_count;
    result->head_formula = &this->head_formula->move();
    return result;
}

void IndexedFilter::init(std::vector<formula::Formula *> const &head_atoms,
                         std::vector<std::string> const &head_variables,
                         std::vector<std::string> const &free_variables,
                         std::vector<std::string> const &bound_variables,
                         std::vector<std::string> const &frontier_variables) {
    this->head_variables = head_variables;
    this->free_variables = free_variables;
    this->bound_variables = bound_variables;
    this->frontier_variables = frontier_variables;
    this->free_variable_index = rule::shared::make_index(free_variables);
    this->bound_variable_index = rule::shared::make_index(bound_variables);
    this->head_formula = build_head_formula(0, head_atoms);
}

void IndexedFilter::update(util::Timeline const &timeline, size_t previous_step,
                           util::Database const &database) {
    auto const &facts = database.get_data_full();
    head_formula->evaluate(timeline, previous_step, facts);
}

std::string IndexedFilter::get_index_key(
    std::shared_ptr<util::Grounding> const &fact) const {
    std::vector<std::string> vector;
    for (auto const &var_name : frontier_variables) {
        auto var_index = free_variable_index.at(var_name);
        auto const &var_value = fact->get_constant(var_index);
        vector.push_back(var_value);
    }
    std::string index_str =
        std::accumulate(vector.begin(), vector.end(), std::string(";"));
    return index_str;
}

void IndexedFilter::clear_index_map() { index_map.clear(); }

void IndexedFilter::compute_index_map(
    std::vector<std::shared_ptr<util::Grounding>> database_facts) {
    for (auto const &db_fact : database_facts) {
        auto key = get_index_key(db_fact);
        index_map.try_emplace(key, db_fact);
    }
}

// std::vector<std::shared_ptr<util::Grounding>>
// IndexedFilter::build_chase_facts( util::Timeline const &timeline, size_t
// previous_step, std::vector<std::shared_ptr<util::Grounding>> const
// &input_facts) { std::vector<std::shared_ptr<util::Grounding>> result;
// std::vector<std::shared_ptr<util::Grounding>> fresh_facts;
// auto current_time = timeline.get_time();
//// filtering by SNE first, then we decide if we need to compute the
//// index map or not.
// for (auto grounding : input_facts) {
// if (grounding->is_fresh_sne(current_time, previous_step)) {
// fresh_facts.push_back(std::move(grounding));
//}
//}
// if (!fresh_facts.empty()) {
// auto database_facts = head_formula->get_conclusions_timepoint(timeline);
// clear_index_map();
// compute_index_map(database_facts);
// for (auto const &input_fact : fresh_facts) {
// if (!has_database_match(input_fact)) {
// auto chase_fact = generate_chase_fact(input_fact);
// result.push_back(chase_fact);
//}
//}
//}
// return result;
//}

std::vector<std::shared_ptr<util::Grounding>> IndexedFilter::build_chase_facts(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &input_facts) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    if (!input_facts.empty()) {
        auto database_facts = head_formula->get_conclusions_timepoint(timeline);
        clear_index_map();
        compute_index_map(database_facts);
        auto current_time = timeline.get_time();
        for (auto const &input_fact : input_facts) {
            if (input_fact->is_fresh_sne(current_time, previous_step) &&
                !has_database_match(input_fact)) {
                auto chase_fact = generate_chase_fact(input_fact);
                result.push_back(chase_fact);
            }
        }
    }
    return result;
}

std::shared_ptr<util::Grounding> IndexedFilter::generate_chase_fact(
    std::shared_ptr<util::Grounding> const &input_fact) {
    std::vector<std::string> chase_values;
    std::vector<std::string> bound_values;
    for (auto const &var_name : bound_variables) {
        auto new_null = generate_new_value(var_name);
        bound_values.push_back(std::move(new_null));
    }
    for (auto const &var_name : head_variables) {
        if (bound_variable_index.count(var_name) > 0) {
            auto index = bound_variable_index.at(var_name);
            auto value = bound_values.at(index);
            chase_values.push_back(value);
        } else {
            auto index = free_variable_index.at(var_name);
            auto value = input_fact->get_constant(index);
            chase_values.push_back(value);
        }
    }
    auto result = input_fact->clone();
    result->set_constant_vector(chase_values);
    return result;
}

std::string IndexedFilter::generate_new_value(std::string const &var_name) {
    std::string result = var_name + std::to_string(null_value_count);
    null_value_count++;
    return result;
}

bool IndexedFilter::has_database_match(
    std::shared_ptr<util::Grounding> const &input_fact) const {
    if (index_map.empty()) {
        return false;
    }
    auto key = get_index_key(input_fact);
    if (index_map.count(key) > 0) {
        auto const &db_fact = index_map.at(key);
        return compare_horizon_time(db_fact, input_fact);
    }
    return false;
}

bool IndexedFilter::compare_horizon_time(
    std::shared_ptr<util::Grounding> const &db_fact,
    std::shared_ptr<util::Grounding> const &input_fact) const {
    // TODO should I check all annotations or is ht enough?
    // return false if there is a match but the new grounding expires later
    return input_fact->get_horizon_time() <= db_fact->get_horizon_time();
}

formula::Formula *IndexedFilter::build_head_formula(
    size_t index, std::vector<formula::Formula *> const &list) const {
    // TODO check if clone is properly implemented in Atom, TimeReference &
    // Conj
    if (index == list.size() - 1) {
        auto result = &list[index]->clone();
        result->set_head(false);
        return result;
    }
    auto left = &list[index]->clone();
    left->set_head(false);
    auto right = build_head_formula(index + 1, list);
    return new formula::Conjunction(left, right, true);
}

void IndexedFilter::expire_outdated_groundings(util::Timeline const &timeline) {
    head_formula->expire_outdated_groundings(timeline);
}

} // namespace laser::rule
