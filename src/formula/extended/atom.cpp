//
// Created by mike on 6/15/18.
//

#include "formula/extended/atom.h"

namespace laser {
namespace formula {

// constructors & destructors

Atom::Atom(std::string predicate) { this->predicate = std::move(predicate); }

Atom::Atom(std::string predicate, std::vector<std::string> variable_names) {
    this->predicate = std::move(predicate);
    set_variable_names(variable_names);
}

void Atom::set_variable_names(std::vector<std::string> &variable_names) {
    grounding_table.set_variable_names(variable_names);
    compute_unique_variable_names(variable_names);
}

void Atom::compute_unique_variable_names(
    std::vector<std::string> &variable_names) {
    has_duplicate_variables = false;
    for (size_t index = 0; index < variable_names.size(); index++) {
        std::string name = variable_names.at(index);
        if (binding_map.count(name) == 0) {
            binding_map.try_emplace(name);
            unique_variable_names.push_back(name);
        }
        auto &vector = binding_map.at(name);
        vector.push_back(index);
    }
    has_duplicate_variables =
        variable_names.size() > unique_variable_names.size();
    // Clean-up
    if (!has_duplicate_variables) {
        binding_map.clear();
    } else {
        // remove entries of non-duplicate variables
        for (auto iterator = binding_map.begin();
             iterator != binding_map.end();) {
            auto const &name = iterator->first;
            auto const &vector = iterator->second;
            if (vector.size() == 1) {
                iterator = binding_map.erase(iterator);
            } else {
                ++iterator;
            }
        }
    }
}

Formula &Atom::create() const {
    auto result = new Atom();
    return *result;
}

Formula &Atom::clone() const {
    Atom *result = new Atom(this->predicate);
    result->is_head_m = this->is_head_m;
    result->type = this->type;
    result->has_duplicate_variables = this->has_duplicate_variables;
    result->unique_variable_names = this->unique_variable_names;
    result->binding_map = this->binding_map;
    result->grounding_table = this->grounding_table;
    return *result;
}

Formula &Atom::move() {
    Atom *result = new Atom(std::move(this->predicate));
    result->is_head_m = this->is_head_m;
    result->type = this->type;
    result->has_duplicate_variables = this->has_duplicate_variables;
    result->unique_variable_names = std::move(this->unique_variable_names);
    result->binding_map = std::move(this->binding_map);
    result->grounding_table = std::move(this->grounding_table);
    return *result;
}

// getters & setters

FormulaType Atom::get_type() const { return this->type; }

std::vector<std::string> Atom::get_predicate_vector() const {
    std::vector<std::string> result;
    result.push_back(this->predicate);
    return result;
}

std::vector<std::string> const &Atom::get_variable_names() const {
    return grounding_table.get_variable_names();
}

void Atom::set_head(bool is_head) { is_head_m = is_head; }

bool Atom::is_head() const { return is_head_m; }

// methods

size_t Atom::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

void Atom::expire_outdated_groundings(util::Timeline const &timeline) {
    grounding_table.expire_outdated_groundings(timeline.get_time(),
                                               timeline.get_tuple_count());
}

std::vector<std::shared_ptr<Grounding>>
Atom::get_groundings(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<Grounding>> Atom::get_conclusion_set() {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<Grounding>>
Atom::get_conclusions_timepoint(util::Timeline const &timeline) {
    return get_conclusion_set();
}

std::vector<std::shared_ptr<Grounding>>
Atom::get_conclusions_step(util::Timeline const &timeline) {
    return grounding_table.get_recent_groundings();
}

bool Atom::evaluate(util::Timeline const &timeline,
                    std::vector<std::shared_ptr<Grounding>> const &facts) {
    for (auto const &grounding : facts) {
        if (grounding->get_predicate() == predicate) {
            accept(grounding);
        }
    }
    return grounding_table.has_recent_groundings();
}

bool Atom::is_valid_fact(Grounding const &grounding) const {
    // Check if duplicate variables have the same constant value in
    // grounding
    if (has_duplicate_variables) {
        for (auto const &iterator : binding_map) {
            auto const &variable_vector = iterator.second;
            auto const &initial = grounding.get_constant(variable_vector.at(0));
            for (auto possition : variable_vector) {
                auto const &current = grounding.get_constant(possition);
                if (current != initial) {
                    return false;
                }
            }
        }
    }
    // Check if sizes match
    auto grounding_size = grounding.get_size();
    auto atom_size = this->get_number_of_variables();
    return grounding_size == atom_size;
}

void Atom::accept(std::shared_ptr<Grounding> const &grounding) {
    if (is_valid_fact(*grounding)) {
        grounding_table.add_grounding(grounding);
    } else {
        // TODO some sort of error
    }
}

int Atom::get_variable_index(std::string const &variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

void Atom::add_child(formula::Formula *child) {}

} // namespace formula
} // namespace laser
