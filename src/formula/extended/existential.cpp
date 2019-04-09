#include "formula/extended/existential.h"

namespace laser {
namespace formula {

Existential::~Existential() { delete child; }

Existential::Existential(std::vector<std::string> argument_vector,
                         Formula *child) {
    this->bound_variables = std::move(argument_vector);
    this->child = &child->move();
    init_variable_vectors();
}

void Existential::init_variable_vectors() {
    child_variables = child->get_variable_names();
    bound_variable_index = make_index(bound_variables);
    for (auto const &var_name : child_variables) {
        if (bound_variable_index.count(var_name) == 0) {
            free_variables.push_back(var_name);
        }
    }
    free_variable_index = make_index(free_variables);
}

std::unordered_map<std::string, int>
Existential::make_index(std::vector<std::string> const &vector) {
    std::unordered_map<std::string, int> result;
    size_t index = 0;
    for (auto const& var_name : vector) {
        result.try_emplace(var_name, index);
        index++;
    }
    return result;
}

Formula &Existential::create() const {
    auto result = new Existential();
    return *result;
}

Formula &Existential::clone() const {
    auto result = new Existential();
    result->child = &this->child->clone();
    result->child_variables = this->child_variables;
    result->bound_variables = this->bound_variables;
    result->free_variables = this->free_variables;
    result->bound_variable_index = this->bound_variable_index;
    result->free_variable_index = this->free_variable_index;
    result->skolem_map = this->skolem_map;
    result->null_value_count = this->null_value_count;
    return *result;
}

Formula &Existential::move() {
    auto result = new Existential();
    result->child = &this->child->move();
    result->child_variables = std::move(this->child_variables);
    result->bound_variables = std::move(this->bound_variables);
    result->free_variables = std::move(this->free_variables);
    result->bound_variable_index = std::move(this->bound_variable_index);
    result->free_variable_index = std::move(this->free_variable_index);
    result->skolem_map = std::move(this->skolem_map);
    result->null_value_count = std::move(this->null_value_count);
    return *result;
}

void Existential::set_head(bool is_head) { child->set_head(is_head); }

bool Existential::is_head() const { return child->is_head(); }

FormulaType Existential::get_type() const { return FormulaType::EXISTENTIAL; }

std::vector<std::string> Existential::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::vector<std::string> const &Existential::get_variable_names() const {
    return free_variables;
}

int Existential::get_variable_index(std::string const &variable_name) const {
    int result = -1;
    if (free_variable_index.count(variable_name) > 0) {
        result = free_variable_index.at(variable_name);
    }
    return result;
}

size_t Existential::get_number_of_variables() const {
    return free_variables.size();
}

std::string Existential::generate_new_value(std::string const &var_name) {
    std::string result = var_name + std::to_string(null_value_count);
    null_value_count++;
    return result;
}

std::shared_ptr<util::Grounding> Existential::make_skolem(
    std::shared_ptr<util::Grounding> const &body_grounding) {
    std::vector<std::string> child_values;
    std::vector<std::string> bound_values;
    auto key = body_grounding->get_hash();
    if (skolem_map.count(key) > 0) {
        bound_values = skolem_map.at(key);
    } else {
        for (auto const &var_name : bound_variables) {
            auto new_null = generate_new_value(var_name);
            bound_values.push_back(std::move(new_null));
        }
        skolem_map.try_emplace(key, bound_values);
    }
    for (auto const &var_name : child_variables) {
        if (bound_variable_index.count(var_name) > 0) {
            auto index = bound_variable_index.at(var_name);
            auto value = bound_values.at(index);
            child_values.push_back(value);
        } else {
            auto index = free_variable_index.at(var_name);
            auto value = body_grounding->get_constant(index);
            child_values.push_back(value);
        }
    }
    return body_grounding->new_constant_vector(child_values);
}

bool Existential::evaluate(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    std::vector<std::shared_ptr<util::Grounding>> skolem_facts;
    for (auto const &grounding : facts) {
        auto skolem_grounding = make_skolem(grounding);
        skolem_facts.push_back(skolem_grounding);
    }
    return child->evaluate(timeline, skolem_facts);
}

void Existential::expire_outdated_groundings(util::Timeline const &timeline) {
    child->expire_outdated_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Existential::get_groundings(util::Timeline const &timeline) {
    auto result = child->get_groundings(timeline);
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Existential::get_conclusions_timepoint(util::Timeline const &timeline) {
    return child->get_conclusions_timepoint(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Existential::get_conclusions_step(util::Timeline const &timeline) {
    return child->get_conclusions_step(timeline);
}

void Existential::add_child(formula::Formula *child) {}

} // namespace formula
} // namespace laser
