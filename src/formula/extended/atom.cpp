#include "formula/extended/atom.h"

namespace laser::formula {

Atom::Atom(std::string predicate) {
    this->arity_map.try_emplace(predicate, 0);
    this->predicate_vector.push_back(std::move(predicate));
}

Atom::Atom(std::string predicate, std::vector<std::string> variable_names) {
    this->arity_map.try_emplace(predicate, variable_names.size());
    this->predicate_vector.push_back(std::move(predicate));
    set_variable_names(variable_names);
}


std::unique_ptr<formula::Formula> Atom::clone() const {
    return std::make_unique<formula::Atom>(*this);
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

FormulaType Atom::get_type() const { return this->type; }

std::vector<std::string> const &Atom::get_predicate_vector() const {
    return predicate_vector;
}

std::map<std::string, size_t> const &Atom::get_arity_map() const {
    return arity_map;
}

std::vector<std::string> const &Atom::get_variable_names() const {
    return grounding_table.get_variable_names();
}

void Atom::set_head(bool is_head) { is_head_m = is_head; }

bool Atom::is_head() const { return is_head_m; }

size_t Atom::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

void Atom::expire_outdated_groundings(util::Timeline const &timeline) {
    auto time = timeline.get_time();
    auto tuple_count = timeline.get_tuple_count_at(time);
    grounding_table.expire_outdated_groundings(time, tuple_count);
}

std::vector<std::shared_ptr<util::Grounding>>
Atom::get_groundings(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<util::Grounding>> Atom::get_conclusion_set() {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<util::Grounding>>
Atom::get_conclusions_timepoint(util::Timeline const &timeline) {
    return get_conclusion_set();
}

std::vector<std::shared_ptr<util::Grounding>>
Atom::get_conclusions_step(util::Timeline const &timeline) {
    return grounding_table.get_recent_groundings();
}

bool Atom::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    auto const &predicate = this->predicate_vector.at(0);
    for (auto const &grounding : facts) {
        std::string const &other_predicate = grounding->get_predicate();
        if (other_predicate == predicate) {
            accept(grounding);
        }
    }
    return grounding_table.has_recent_groundings();
}

bool Atom::is_valid_fact(util::Grounding const &grounding) const {
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

void Atom::accept(std::shared_ptr<util::Grounding> const &grounding) {
    if (is_valid_fact(*grounding)) {
        grounding_table.add_grounding(grounding);
    } else {
        // TODO some sort of error
    }
}

int Atom::get_variable_index(std::string const &variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

void Atom::add_child(std::unique_ptr<formula::Formula> child) {}

std::vector<std::unique_ptr<formula::Formula> const *>
Atom::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    return result;
}

uint64_t Atom::get_window_size() const { return 0; }

} // namespace laser::formula
