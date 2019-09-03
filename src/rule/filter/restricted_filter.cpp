#include "rule/filter/restricted_filter.h"

namespace laser::rule {

ChaseFilter *RestrictedFilter::create() const {
    auto result = new RestrictedFilter();
    return result;
}

RestrictedFilter::~RestrictedFilter() { delete head_formula; }

ChaseFilter *RestrictedFilter::clone() const {
    auto result = new RestrictedFilter();
    result->frontier_variables = this->frontier_variables;
    result->head_variables = this->head_variables;
    result->free_variables = this->free_variables;
    result->bound_variables = this->bound_variables;
    result->free_variable_index = this->free_variable_index;
    result->bound_variable_index = this->bound_variable_index;
    result->null_value_count = this->null_value_count;
    result->head_formula = &this->head_formula->clone();
    result->has_inertia_variables = this->has_inertia_variables;
    result->is_inertia_variable = this->is_inertia_variable;
    result->current_timepoint = this->current_timepoint;
    result->inertia_facts = this->inertia_facts;
    result->current_facts = this->current_facts;
    result->facts_found_in_db = this->facts_found_in_db;
    return result;
}

ChaseFilter *RestrictedFilter::move() {
    auto result = new RestrictedFilter();
    result->frontier_variables = std::move(this->frontier_variables);
    result->head_variables = std::move(this->head_variables);
    result->free_variables = std::move(this->free_variables);
    result->bound_variables = std::move(this->bound_variables);
    result->free_variable_index = std::move(this->free_variable_index);
    result->bound_variable_index = std::move(this->bound_variable_index);
    result->null_value_count = this->null_value_count;
    result->head_formula = &this->head_formula->move();
    result->has_inertia_variables = this->has_inertia_variables;
    result->is_inertia_variable = std::move(this->is_inertia_variable);
    result->current_timepoint = this->current_timepoint;
    result->inertia_facts = std::move(this->inertia_facts);
    result->current_facts = std::move(this->current_facts);
    result->facts_found_in_db = std::move(this->facts_found_in_db);
    return result;
}

void RestrictedFilter::init(std::vector<formula::Formula *> const &head_atoms,
                            std::vector<std::string> const &head_variables,
                            std::vector<std::string> const &free_variables,
                            std::vector<std::string> const &bound_variables,
                            std::vector<bool> const &is_inertia_variable,
                            std::vector<std::string> const &frontier_variables,
                            bool has_inertia_variables) {
    this->head_variables = head_variables;
    this->free_variables = free_variables;
    this->bound_variables = bound_variables;
    this->frontier_variables = frontier_variables;
    this->free_variable_index = rule::shared::make_index(free_variables);
    this->bound_variable_index = rule::shared::make_index(bound_variables);
    this->head_formula = build_head_formula(0, head_atoms);
    this->has_inertia_variables = has_inertia_variables;
    this->is_inertia_variable = is_inertia_variable;
}

void RestrictedFilter::update(util::Timeline const &timeline,
                              size_t previous_step,
                              util::Database const &database) {
    auto const &facts = database.get_data_full();
    head_formula->evaluate(timeline, previous_step, facts);
    // TODO we might want to store results in a grounding table, since we don't
    // want some conclusions to be skipped due to SNE. Alternative, disable
    // SNE in conjunction if (is_head_m == true) {}
    // If we do use local grounding table, get conclusions here using
    // head_formula.get_conclusions_step(timeline);
    auto new_time = timeline.get_time();
    if (new_time > current_timepoint) {
        if (has_inertia_variables) {
            inertia_facts.clear();
            inertia_facts = current_facts;
            //inertia_facts.insert(inertia_facts.end(), facts_found_in_db.begin(),
                                 //facts_found_in_db.end());
        }
        current_facts.clear();
        facts_found_in_db.clear();
        current_timepoint = new_time;
    }
}

std::vector<std::shared_ptr<util::Grounding>>
RestrictedFilter::build_chase_facts(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &input_facts) {
    auto database_facts = head_formula->get_conclusions_timepoint(timeline);
    auto current_time = timeline.get_time();
    for (auto const &input_fact : input_facts) {
        if (input_fact->is_fresh_sne(current_time, previous_step)) {
            find_match(database_facts, input_fact);
        }
    }
    return current_facts;
}

std::shared_ptr<util::Grounding> RestrictedFilter::convert_to_chase_fact(
    std::shared_ptr<util::Grounding> const &db_fact) {
    std::vector<std::string> chase_values;
    for (auto const &var_name : head_variables) {
        int head_index = head_formula->get_variable_index(var_name);
        if (head_index >= 0) {
            auto value = db_fact->get_constant(head_index);
            chase_values.push_back(value);
        } else {
            chase_values.push_back("filler_non-frontier_value");
        }
    }
    auto result = db_fact->shallow_clone();
    result->set_constant_vector(chase_values);
    return result;
}

std::shared_ptr<util::Grounding> RestrictedFilter::generate_chase_fact(
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
    auto result = input_fact->shallow_clone();
    result->set_constant_vector(chase_values);
    return result;
}

std::string RestrictedFilter::generate_new_value(std::string const &var_name) {
    // TODO
    // The value needs to be more random, as is two rules have the same
    // existential quantified variable names, the same null value will be
    // generated by diferent rules.
    // But introducing random names complicates automatic testing.
    // TODO
    // TODO also make sure diferent rules don't generate the same value
    std::string result = var_name + std::to_string(null_value_count);
    null_value_count++;
    return result;
}

std::shared_ptr<util::Grounding>
RestrictedFilter::generate_chase_fact_from_inertia(
    std::shared_ptr<util::Grounding> const &inertia_fact) {
    std::vector<std::string> chase_values;
    for (size_t i = 0; i < head_variables.size(); i++) {
        auto const &var_name = head_variables.at(i);
        std::string value;
        if ((bound_variable_index.count(var_name) > 0) &&
            (!is_inertia_variable.at(bound_variable_index.at(var_name)))) {
            value = generate_new_value(var_name);
        } else {
            value = inertia_fact->get_constant(i);
        }
        chase_values.push_back(value);
    }
    auto result = inertia_fact->shallow_clone();
    result->set_constant_vector(chase_values);
    if (result->get_horizon_time() < current_timepoint) {
        result->set_horizon_time(current_timepoint);
    }
    return result;
}

void RestrictedFilter::find_match(
    std::vector<std::shared_ptr<util::Grounding>> const &database,
    std::shared_ptr<util::Grounding> const &input_fact) {
    if (has_inertia_variables) {
        for (auto const &inertia_fact : inertia_facts) {
            if (is_inertia_variable_match(inertia_fact, input_fact)) {
                auto new_fact = generate_chase_fact_from_inertia(inertia_fact);
                current_facts.push_back(new_fact);
                return;
            }
        }
    } else {
        for (auto const &db_fact : database) {
            if (is_database_match(db_fact, input_fact)) {
                auto new_fact = convert_to_chase_fact(db_fact);
                //facts_found_in_db.push_back(new_fact);
                return;
            }
        }
    }
    auto chase_fact = generate_chase_fact(input_fact);
    current_facts.push_back(chase_fact);
}

bool RestrictedFilter::is_database_match(
    std::shared_ptr<util::Grounding> const &db_fact,
    std::shared_ptr<util::Grounding> const &input_fact) const {
    // return false if there is a match but the new grounding expires later
    return is_frontier_variable_match(db_fact, input_fact) &&
           input_fact->get_horizon_time() <= db_fact->get_horizon_time();
}

bool RestrictedFilter::is_inertia_variable_match(
    std::shared_ptr<util::Grounding> const &inertia_fact,
    std::shared_ptr<util::Grounding> const &input_fact) const {
    // TODO should I check all annotations or is ht enough?
    for (auto const &var_name : frontier_variables) {
        auto input_index = free_variable_index.at(var_name);
        auto const &input_value = input_fact->get_constant(input_index);
        auto const &inertia_value = inertia_fact->get_constant(input_index);
        if (inertia_value != input_value) {
            return false;
        }
    }
    return true;
}

bool RestrictedFilter::is_frontier_variable_match(
    std::shared_ptr<util::Grounding> const &db_fact,
    std::shared_ptr<util::Grounding> const &input_fact) const {
    // TODO should I check all annotations or is ht enough?
    for (auto const &var_name : frontier_variables) {
        auto input_index = free_variable_index.at(var_name);
        auto const &input_value = input_fact->get_constant(input_index);
        auto db_index = head_formula->get_variable_index(var_name);
        auto const &db_value = db_fact->get_constant(db_index);
        if (db_value != input_value) {
            return false;
        }
    }
    return true;
}

formula::Formula *RestrictedFilter::build_head_formula(
    size_t index, std::vector<formula::Formula *> const &list) const {
    // TODO check if clone is properly implemented in Atom, TimeReference & Conj
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

void RestrictedFilter::expire_outdated_groundings(
    util::Timeline const &timeline) {
    head_formula->expire_outdated_groundings(timeline);
}

} // namespace laser::rule
