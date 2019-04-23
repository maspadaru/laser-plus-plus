#include "formula/extended/existential_restricted.h"

namespace laser {
namespace formula {

ExistentialRestricted::~ExistentialRestricted() {
    for (auto child : children) {
        delete child;
    }
    children.clear();
}

ExistentialRestricted::ExistentialRestricted(std::vector<std::string> argument_vector,
                         std::vector<Formula *> children) {
    this->bound_variables = std::move(argument_vector);
    this->children = children;
    init_variable_vectors();
}

void ExistentialRestricted::init_variable_vectors() {
    std::set<std::string> child_variable_set;
    std::set<std::string> atom_variable_set;
    for (auto child : children) {
        child->set_head(true);
        auto variable_names = child->get_variable_names();
        predicate_vector.push_back(child->get_predicate_vector()[0]);
        std::copy(variable_names.begin(), variable_names.end(),
                  std::inserter(child_variable_set, child_variable_set.end()));
        if (child->get_type() == FormulaType::TIME_REFERENCE) {
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
    std::copy(child_variable_set.begin(), child_variable_set.end(),
              std::back_inserter(child_variables));
    child_variable_index = make_index(child_variables);
    bound_variable_index = make_index(bound_variables);
    for (auto const &var_name : child_variables) {
        if (bound_variable_index.count(var_name) == 0) {
            free_variables.push_back(var_name);
        }
    }
    free_variable_index = make_index(free_variables);
    for (auto const &variable_name : bound_variables) {
        atom_variable_set.erase(variable_name);
    }
    std::copy(atom_variable_set.begin(), atom_variable_set.end(),
              std::back_inserter(atom_variables));
}

std::unordered_map<std::string, int>
ExistentialRestricted::make_index(std::vector<std::string> const &vector) {
    std::unordered_map<std::string, int> result;
    size_t index = 0;
    for (auto const &var_name : vector) {
        result.try_emplace(var_name, index);
        index++;
    }
    return result;
}

Formula &ExistentialRestricted::create() const {
    auto result = new ExistentialRestricted();
    return *result;
}

Formula &ExistentialRestricted::clone() const {
    auto result = new ExistentialRestricted();
    result->children = this->children;
    result->predicate_vector = this->predicate_vector;
    result->child_variables = this->child_variables;
    result->bound_variables = this->bound_variables;
    result->free_variables = this->free_variables;
    result->atom_variables = this->atom_variables;
    result->child_variable_index = this->child_variable_index;
    result->bound_variable_index = this->bound_variable_index;
    result->free_variable_index = this->free_variable_index;
    result->skolem_map = this->skolem_map;
    result->null_value_count = this->null_value_count;
    return *result;
}

Formula &ExistentialRestricted::move() {
    auto result = new ExistentialRestricted();
    result->children = std::move(this->children);
    result->predicate_vector = std::move(this->predicate_vector);
    result->child_variables = std::move(this->child_variables);
    result->bound_variables = std::move(this->bound_variables);
    result->free_variables = std::move(this->free_variables);
    result->atom_variables = std::move(this->atom_variables);
    result->child_variable_index = std::move(this->child_variable_index);
    result->bound_variable_index = std::move(this->bound_variable_index);
    result->free_variable_index = std::move(this->free_variable_index);
    result->skolem_map = std::move(this->skolem_map);
    result->null_value_count = this->null_value_count;
    return *result;
}

void ExistentialRestricted::set_head(bool is_head) {}

bool ExistentialRestricted::is_head() const { return true; }

FormulaType ExistentialRestricted::get_type() const { return FormulaType::EXISTENTIAL; }

std::vector<std::string> ExistentialRestricted::get_predicate_vector() const {
    return predicate_vector;
}

std::vector<std::string> const &ExistentialRestricted::get_variable_names() const {
    return free_variables;
}

int ExistentialRestricted::get_variable_index(std::string const &variable_name) const {
    int result = -1;
    if (free_variable_index.count(variable_name) > 0) {
        result = free_variable_index.at(variable_name);
    }
    return result;
}

size_t ExistentialRestricted::get_number_of_variables() const {
    return free_variables.size();
}

std::string ExistentialRestricted::generate_new_value(std::string const &var_name) {
    // TODO
    // The value needs to be more random, as is two rules have the same
    // existential quantified variable names, the same null value will be
    // generated by diferent rules.
    // But introducing random names complicates automatic testing.
    // TODO
    std::string result = var_name + std::to_string(null_value_count);
    null_value_count++;
    return result;
}

std::shared_ptr<util::Grounding> ExistentialRestricted::make_skolem(
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

void ExistentialRestricted::evaluate_database_conclusions(
    util::Timeline const &timeline, util::Database const &database) {
    auto const &facts = database.get_data_full();
    for (auto child : children) {
        if (child->get_type() == FormulaType::TIME_REFERENCE) {
            child->set_head(false);
            child->evaluate(timeline, database, facts);
            child->set_head(true);
        } else {
            child->evaluate(timeline, database, facts);
        }
    }
}

std::vector<std::string> ExistentialRestricted::make_head_vector(
    std::vector<std::string> bound_values,
    std::shared_ptr<util::Grounding> const &grounding) const {
    std::vector<std::string> result;
    for (auto const &var_name : child_variables) {
        if (bound_variable_index.count(var_name) > 0) {
            auto index = bound_variable_index.at(var_name);
            auto value = bound_values.at(index);
            result.push_back(value);
        } else {
            auto index = free_variable_index.at(var_name);
            auto value = grounding->get_constant(index);
            result.push_back(value);
        }
    }
    return result;
}

bool ExistentialRestricted::is_predicate_in_head(
    std::shared_ptr<util::Grounding> const &body_grounding) {
    auto const &predicate = body_grounding->get_predicate();
    for (auto child : children) {
        auto child_predicate = child->get_predicate_vector()[0];
        if (predicate == child_predicate) {
            return true;
        }
    }
    return false;
}

bool ExistentialRestricted::evaluate(
    util::Timeline const &timeline, util::Database const &database,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    evaluate_database_conclusions(timeline, database);
    std::vector<std::shared_ptr<util::Grounding>> skolem_facts;
    for (auto const &body_grounding : facts) {
            /*TODO I think is_predicate_in_head is a useless check
             * since all the body_groundings have the same predicate 
             * as the head. 
             */
        if (is_predicate_in_head(body_grounding)) { 
            auto key = body_grounding->get_hash();
            bool has_match = skolem_map.count(key) > 0;
            if (has_match) {
                auto const &bound_values = skolem_map.at(key);
                auto head_values =
                    make_head_vector(bound_values, body_grounding);
                for (auto child : children) {
                    has_match &=
                        has_database_match(timeline, child, head_values);
                }
            } else {
                //TODO e.g. for Bicycle(w0), skolem_map.count == 0;
                //so we need to go through all atoms, and find matches for w0
                //and everytime we find a match, we need to check 
                //has_database_match on all children, except for this one.



            }
            if (!has_match) {
                auto skolem_grounding = make_skolem(body_grounding);
                skolem_facts.push_back(skolem_grounding);
            }
        }
    }
    for (auto child : children) {
        std::vector<std::shared_ptr<util::Grounding>> child_facts;
        for (auto const &grounding : skolem_facts) {
            auto child_fact = make_child_fact(grounding, child);
            child_facts.push_back(child_fact);
        }
        child->evaluate(timeline, database, child_facts);
    }
    return true;
}

std::shared_ptr<util::Grounding> ExistentialRestricted::make_child_fact(
    std::shared_ptr<util::Grounding> const &skolem_fact, Formula *child) const {
    auto child_variables = child->get_variable_names();
    auto predicate = child->get_predicate_vector().at(0);
    std::vector<std::string> child_values;
    for (auto const &var : child_variables) {
        auto position = child_variable_index.at(var);
        auto const &value = skolem_fact->get_constant(position);
        child_values.push_back(value);
    }
    return skolem_fact->new_pred_constvec(predicate, child_values);
}

bool ExistentialRestricted::has_database_match(
    util::Timeline const &timeline, Formula *child,
    std::vector<std::string> const &value_vector) const {
    auto const &data = child->get_groundings(timeline);
    for (auto const &db_grounding : data) {
        if (is_variable_match(child, db_grounding, value_vector)) {
            return true;
        }
    }
    return false;
}

bool ExistentialRestricted::is_variable_match(
    Formula *child, std::shared_ptr<util::Grounding> const &child_grounding,
    std::vector<std::string> const &input_value_vector) const {
    auto child_variables = child->get_variable_names();
    size_t child_index = 0;
    for (auto const &var_name : child_variables) {
        auto input_index = child_variable_index.at(var_name);
        auto const &input_value = input_value_vector.at(input_index);
        auto const &child_value = child_grounding->get_constant(child_index);
        child_index++;
        if (child_value != input_value) {
            return false;
        }
    }
    return true;
}

void ExistentialRestricted::expire_outdated_groundings(util::Timeline const &timeline) {
    for (auto child : children) {
        child->expire_outdated_groundings(timeline);
    }
}

std::vector<std::shared_ptr<util::Grounding>>
ExistentialRestricted::get_groundings(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto child : children) {
        auto child_groundings = child->get_groundings(timeline);
        result.insert(result.end(), child_groundings.begin(),
                      child_groundings.end());
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
ExistentialRestricted::get_conclusions_timepoint(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto child : children) {
        auto child_groundings = child->get_conclusions_timepoint(timeline);
        result.insert(result.end(), child_groundings.begin(),
                      child_groundings.end());
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
ExistentialRestricted::get_conclusions_step(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto child : children) {
        auto child_groundings = child->get_conclusions_step(timeline);
        result.insert(result.end(), child_groundings.begin(),
                      child_groundings.end());
    }
    return result;
}

void ExistentialRestricted::add_child(formula::Formula *child) {
    children.push_back(child);
    init_variable_vectors();
}

} // namespace formula
} // namespace laser