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

// getters & setters

FormulaType Atom::get_type() const { return this->type; }

std::string Atom::get_predicate() const { return predicate; }

bool Atom::is_negated() const { return is_negated_m; }

bool Atom::had_input_already() const { return had_input_already_m; }

// const methods

bool Atom::holds(long long int current_time) const {
    // TODO implement
    return true;
}

// methods

void Atom::accept(std::vector<Formula> facts, long long int current_time,
                  long long int current_tuple_counter) {
    // TODO implement
}

} // namespace formula
} // namespace laser

