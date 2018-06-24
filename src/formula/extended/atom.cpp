//
// Created by mike on 6/15/18.
//

#include <iostream>
#include "formula/extended/atom.h"

namespace laser {
namespace formula {

// constructors & destructors

Atom::Atom(std::string predicate) {
    this->predicate = std::move(predicate);
    std::cerr << "CREATE Atom -> predicate:  " << this->predicate << std::endl;
}

Atom::Atom(std::string predicate, std::vector<std::string> variable_names) {
    this->predicate = std::move(predicate);
    this->variable_names = std::move(variable_names);
    std::cerr << "CREATE Atom + var_names -> predicate:  " << this->predicate << std::endl;

    std::cerr << "variable_names.size=: " << this->variable_names.size() << "; Names: ";
    for(size_t i = 0; i < this->variable_names.size(); i++) {
        auto name = this->variable_names.at(i);
        std::cerr << name << ", ";
    }
    std::cerr <<"DONE;"<<  std::endl;

}

Formula &Atom::create() const {
    std::cerr << "Atom.create() -> " << this->predicate << std::endl;
    auto result = new Atom();
    return *result;
}

Formula &Atom::clone() const {
    std::cerr << "Atom.clone() -> " << this->predicate << std::endl;
    auto result = new Atom(*this);
    return *result;
}

Formula &Atom::move() {
    std::cerr << "Atom.move() -> ";
    Atom *result = new Atom(this->predicate);
    result->variable_names = std::move(this->variable_names);
    // TODO: Uncomment the line bellow:  Delete the two lines above
    // Atom *result = new Atom(std::move(this->predicate), std::move(this->variable_names));
    result->type = this->type;
    result->is_negated_m = this->is_negated_m;
    result->grounding_table = std::move(this->grounding_table);
    result->result_grounding_table = std::move(result_grounding_table);
    return *result;
}

Atom::~Atom() {
    std::cerr << "DELETE Atom -> predicate:  " << this->get_predicate() << std::endl;
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

void Atom::accept(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter,
        std::vector<Formula *> facts) {
    // TODO implement
}

size_t Atom::get_number_of_variables() const {
    return variable_names.size();
}

void Atom::expire_outdated_groundings(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {
    grounding_table.expire_outdated_groundings(current_time,
            current_tuple_counter);
}


} // namespace formula
} // namespace laser

