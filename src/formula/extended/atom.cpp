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

Formula &Atom::create() const {
    auto result = new Atom();
    return *result;
}

Formula &Atom::clone() const {
    auto result = new Atom(*this);
    return *result;
}

Formula &Atom::move() {
    Atom *result = new Atom(std::move(this->predicate), std::move(this->variable_names));
    result->type = this->type;
    result->is_negated_m = this->is_negated_m;
    result->grounding_table = std::move(this->grounding_table);
    result->result_grounding_table = std::move(result_grounding_table);
    return *result;
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

bool Atom::holds(uint64_t current_time) const {
    // TODO implement
    return true;
}

// methods

void Atom::accept(
        uint64_t current_time,
        uint64_t current_tuple_counter,
        std::vector<Formula *> facts) {
    // TODO implement
}

size_t Atom::get_number_of_variables() const {
    return variable_names.size();
}

void Atom::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    grounding_table.expire_outdated_groundings(current_time,
            current_tuple_counter);
}

void Atom::debug_print() const {
    std::cerr << "ATOM -> predicate:  " << this->predicate ;
    std::cerr << "; variable_names.size=: " << this->variable_names.size() << "; Names: ";
    for(size_t i = 0; i < this->variable_names.size(); i++) {
        auto name = this->variable_names.at(i);
        std::cerr << name << ", ";
    }
    std::cerr << "/" << std::endl;
}

void Atom::add_grounding(
        uint64_t consideration_time, uint64_t horizon_time,
        uint64_t consideration_count, uint64_t horizon_count,
        std::vector<std::string> arguments) {
        laser::formula::Grounding grounding(consideration_time, horizon_time,
                consideration_count, horizon_count);
        for (int i=0; i < arguments.size(); i++) {
            grounding.add_substitution(i, arguments.at(i));
        }
        this->grounding_table.add_grounding(grounding);

}


} // namespace formula
} // namespace laser

