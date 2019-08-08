#include "rule/filter/skolem_filter.h"

namespace laser {
namespace rule {

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
    result->free_variable_index = this->free_variable_index;
    result->bound_variable_index = this->bound_variable_index;
    result->skolem_map = this->skolem_map;
    result->null_value_count = this->null_value_count;
    return result;
}

ChaseFilter *SkolemFilter::move() {
    auto result = new SkolemFilter();
    result->head_variables = std::move(this->head_variables);
    result->free_variables = std::move(this->free_variables);
    result->frontier_variables = std::move(this->frontier_variables);
    result->bound_variables = std::move(this->bound_variables);
    result->free_variable_index = std::move(this->free_variable_index);
    result->bound_variable_index = std::move(this->bound_variable_index);
    result->skolem_map = std::move(this->skolem_map);
    result->null_value_count = this->null_value_count;
    return result;
}

void SkolemFilter::init_frontier_variables(
    std::vector<formula::Formula *> const &head_atoms) {
    // I am igniring the time variable here, else I will always get new values
    // at each timepoint
    std::set<std::string> atom_variable_set;
    for (auto atom : head_atoms) {
        auto variable_names = atom->get_variable_names();
        if (atom->get_type() == formula::FormulaType::TIME_REFERENCE) {
            // Time variable is always the last
            std::copy(
                variable_names.begin(), variable_names.end() - 1,
                std::inserter(atom_variable_set, atom_variable_set.end()));
        } else {
            std::copy(
                variable_names.begin(), variable_names.end(),
                std::inserter(atom_variable_set, atom_variable_set.end()));
        }
    }

    for (auto const &variable : bound_variables) {
        atom_variable_set.erase(variable);
    }
    std::copy(atom_variable_set.begin(), atom_variable_set.end(),
              std::back_inserter(frontier_variables));
}

void SkolemFilter::init(std::vector<formula::Formula *> const &head_atoms,
                        std::vector<std::string> const &head_variables,
                        std::vector<std::string> const &free_variables,
                        std::vector<std::string> const &bound_variables) {
    this->head_variables = head_variables;
    this->free_variables = free_variables;
    this->bound_variables = bound_variables;
    this->free_variable_index = rule::shared::make_index(free_variables);
    this->bound_variable_index = rule::shared::make_index(bound_variables);
    init_frontier_variables(head_atoms);
}

void SkolemFilter::update(util::Timeline const &timeline, size_t previous_step,
                          util::Database const &database) {
    return;
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
    std::string hash_str =
        std::accumulate(key_values.begin(), key_values.end(), std::string(";"));
    return hash_str;
}

std::shared_ptr<util::Grounding> SkolemFilter::generate_chase_fact(
    std::shared_ptr<util::Grounding> const &input_fact) {
    std::vector<std::string> child_values;
    std::vector<std::string> bound_values;
    auto skolem_key = compute_key(input_fact);
    if (skolem_map.count(skolem_key) > 0) {
        bound_values = skolem_map.at(skolem_key);
    } else {
        for (auto const &var_name : bound_variables) {
            auto new_null = generate_new_value(var_name);
            bound_values.push_back(std::move(new_null));
        }
        skolem_map.try_emplace(skolem_key, bound_values);
    }
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
    auto result = input_fact->clone();
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

} // namespace rule
} // namespace laser
