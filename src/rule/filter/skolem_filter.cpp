#include "rule/filter/skolem_filter.h"

namespace laser::rule {

ChaseFilter *SkolemFilter::create() const {
    auto result = new SkolemFilter();
    return result;
}

ChaseFilter *SkolemFilter::clone() const {
    auto result = new SkolemFilter();
    result->head_variables = this->head_variables;
    result->free_variables = this->free_variables;
    result->frontier_variables = this->frontier_variables;
    result->bound_variables = this->bound_variables;
    result->is_inertia_variable = this->is_inertia_variable;
    result->free_variable_index = this->free_variable_index;
    result->bound_variable_index = this->bound_variable_index;
    result->skolem_map = this->skolem_map;
    result->null_value_count = this->null_value_count;
    result->has_inertia_variables = this->has_inertia_variables;
    result->current_timepoint = this->current_timepoint;
    result->skolem_map = this->skolem_map;
    result->inertia_map = this->inertia_map;
    return result;
}

ChaseFilter *SkolemFilter::move() {
    auto result = new SkolemFilter();
    result->head_variables = std::move(this->head_variables);
    result->free_variables = std::move(this->free_variables);
    result->frontier_variables = std::move(this->frontier_variables);
    result->bound_variables = std::move(this->bound_variables);
    result->is_inertia_variable = std::move(this->is_inertia_variable);
    result->free_variable_index = std::move(this->free_variable_index);
    result->bound_variable_index = std::move(this->bound_variable_index);
    result->skolem_map = std::move(this->skolem_map);
    result->null_value_count = this->null_value_count;
    result->has_inertia_variables = this->has_inertia_variables;
    result->current_timepoint = this->current_timepoint;
    result->skolem_map = std::move(this->skolem_map);
    result->inertia_map = std::move(this->inertia_map);
    return result;
}

void SkolemFilter::init(std::vector<formula::Formula *> const &head_atoms,
                        std::vector<std::string> const &head_variables,
                        std::vector<std::string> const &free_variables,
                        std::vector<std::string> const &bound_variables,
                        std::vector<bool> const &is_inertia_variable,
                        std::vector<std::string> const &frontier_variables,
                        bool has_inertia_variables) {
    this->head_variables = head_variables;
    this->free_variables = free_variables;
    this->bound_variables = bound_variables;
    this->is_inertia_variable = is_inertia_variable;
    this->frontier_variables = frontier_variables;
    this->free_variable_index = rule::shared::make_index(free_variables);
    this->bound_variable_index = rule::shared::make_index(bound_variables);
    this->has_inertia_variables = has_inertia_variables;
}

void SkolemFilter::update(util::Timeline const &timeline, size_t previous_step,
                          util::Database const &database) {
    auto new_time = timeline.get_time();
    if (new_time > current_timepoint) {
        if (has_inertia_variables) {
            inertia_map.clear();
            inertia_map = skolem_map;
        }
        skolem_map.clear();
        current_timepoint = new_time;
    }
}

std::vector<std::shared_ptr<util::Grounding>> SkolemFilter::build_chase_facts(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &input_facts) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    auto current_time = timeline.get_time();
    for (auto const &input_fact : input_facts) {
        if (input_fact->is_fresh_sne(current_time, previous_step)) {
            auto chase_fact = generate_chase_fact(input_fact);
            result.push_back(chase_fact);
        }
    }
    return result;
}

std::string SkolemFilter::concatenate(std::vector<std::string> const &source,
                                      std::string const &delimiter) const {
    // Maybe accumulate is faster, but with accumulate I can't set a delimiter
    std::stringstream destination;
    copy(source.begin(), source.end(),
         std::ostream_iterator<std::string>(destination, delimiter.c_str()));
    return destination.str();
}

std::string SkolemFilter::compute_key(
    std::shared_ptr<util::Grounding> const &input_fact) const {
    // The input_fact contains values for all free variables.
    // We need to extract only the fronteer values
    // and generate a new grounding.
    std::vector<std::string> key_values;
    for (auto const &var_name : frontier_variables) {
        auto index = free_variable_index.at(var_name);
        auto value = input_fact->get_constant(index);
        key_values.push_back(value);
    }
    auto result = concatenate(key_values, ";;");
    return result;
}

std::vector<std::string> const &
SkolemFilter::generate_new_values(std::string const &skolem_key) {
    std::vector<std::string> new_values;
    for (auto const &var_name : bound_variables) {
        auto new_null = generate_new_value(var_name);
        new_values.push_back(std::move(new_null));
    }
    skolem_map.try_emplace(skolem_key, new_values);
    return skolem_map.at(skolem_key);
}

std::vector<std::string> const &
SkolemFilter::get_bound_values_with_inertia(std::string const &skolem_key) {
    std::vector<std::string> new_values;
    std::vector<std::string> const &inertia_vector = inertia_map.at(skolem_key);
    for (size_t var_index = 0; var_index < bound_variables.size();
         var_index++) {
        auto const &var_name = bound_variables.at(var_index);
        if (is_inertia_variable.at(var_index)) {
            auto const &var_value = inertia_vector.at(var_index);
            new_values.push_back(std::move(var_value));
        } else {
            auto new_null = generate_new_value(var_name);
            new_values.push_back(std::move(new_null));
        }
    }
    skolem_map.try_emplace(skolem_key, new_values);
    return skolem_map.at(skolem_key);
}

std::vector<std::string> const &
SkolemFilter::get_bound_values(std::string const &skolem_key) {
    if (skolem_map.count(skolem_key) > 0) {
        return skolem_map.at(skolem_key);
    } else if (has_inertia_variables && inertia_map.count(skolem_key) > 0) {
        return get_bound_values_with_inertia(skolem_key);
    }
    return generate_new_values(skolem_key);
}

std::shared_ptr<util::Grounding> SkolemFilter::generate_chase_fact(
    std::shared_ptr<util::Grounding> const &input_fact) {
    std::vector<std::string> child_values;
    auto skolem_key = compute_key(input_fact);
    auto const &bound_values = get_bound_values(skolem_key);
    for (auto const &var_name : head_variables) {
        if (bound_variable_index.count(var_name) > 0) {
            auto index = bound_variable_index.at(var_name);
            auto value = bound_values.at(index);
            child_values.push_back(value);
        } else {
            auto index = free_variable_index.at(var_name);
            auto value = input_fact->get_constant(index);
            child_values.push_back(value);
        }
    }
    auto result = input_fact->shallow_clone();
    result->set_constant_vector(child_values);
    return result;
}

std::string SkolemFilter::generate_new_value(std::string const &var_name) {
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

void SkolemFilter::expire_outdated_groundings(util::Timeline const &timeline) {}

} // namespace laser::rule
