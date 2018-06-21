//
// Created by mike on 6/15/18.
//

#include "formula/extended/atom.h"

namespace laser {
namespace formula {

// constructors & destructors

Atom::Atom(std::string predicate) {
    this->predicate = std::move(predicate);
}

Atom::Atom(std::string predicate, std::vector<std::string> variable_names) {
    this->predicate = std::move(predicate);
    this->variable_names = std::move(variable_names);
}

// getters & setters

FormulaType Atom::get_type() const { return this->type; }

std::string Atom::get_predicate() const { return predicate; }

bool Atom::is_negated() const { return is_negated_m; }

bool Atom::had_input_already() const { return had_input_already_m; }

std::vector<std::string> Atom::get_variable_names() const {
    return variable_names;
}

// const methods

bool Atom::holds(unsigned long long int current_time) const {
    // TODO implement
    return true;
}

// methods

void Atom::accept(unsigned long long int current_time,
                  unsigned long long int current_tuple_counter,
                  std::vector<Formula *> facts) {
    // TODO implement
}

size_t Atom::get_number_of_variables() const {
    return variable_names.size();
}

void Atom::expire_outdated_groundings(unsigned long long int current_time,
                                      unsigned long long int current_tuple_counter) {
    grounding_table.expire_outdated_groundings(current_time,
            current_tuple_counter);
}

} // namespace formula
} // namespace laser

