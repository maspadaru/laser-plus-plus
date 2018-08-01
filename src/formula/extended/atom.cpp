//
// Created by mike on 6/15/18.
//

#include <utility>
#include "formula/extended/atom.h"

namespace laser {
namespace formula {

// constructors & destructors

Atom::Atom(std::string predicate) {
    this->predicate = std::move(predicate);
}

Atom::Atom(std::string predicate, std::vector<std::string> variable_names) {
    this->predicate = std::move(predicate);
    this->grounding_table.set_variable_names(std::move(variable_names));
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
    Atom *result = new Atom(std::move(this->predicate));
    result->type = this->type;
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

std::vector<std::string> Atom::get_variable_names() const {
    return grounding_table.get_variable_names();
}

// methods


size_t Atom::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

void Atom::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    grounding_table.expire_outdated_groundings(current_time,
            current_tuple_counter);
}

void Atom::debug_print() const {
    std::cerr << "ATOM -> predicate:  " << this->predicate ;
    std::cerr << "; variable_names.size=: " << this->get_number_of_variables() << "; Names: ";
    for(auto const &variable : this->get_variable_names()) {
        std::cerr << variable << ", ";
    }
    std::cerr << "/" << std::endl;
}

void Atom::add_grounding(
        uint64_t consideration_time, uint64_t horizon_time,
        uint64_t consideration_count, uint64_t horizon_count,
        std::vector<std::string> arguments) {
        laser::formula::Grounding grounding(consideration_time, horizon_time,
                consideration_count, horizon_count);
        for (size_t i = 0; i < arguments.size(); i++) {
            grounding.add_substitution(i, arguments.at(i));
        }
        this->grounding_table.add_grounding(grounding);

}

void Atom::add_grounding(Grounding grounding) {
    grounding_table.add_grounding(std::move(grounding));
}

std::vector<Grounding> Atom::get_groundings() {
    return grounding_table.get_all_groundings();
}

bool Atom::is_satisfied() const {
    return grounding_table.get_size() > 0;
}

bool Atom::evaluate(
        uint64_t current_time,
        uint64_t current_tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        facts) {
    bool result = false;

    auto search = facts.find(predicate);
    auto found = search != facts.end();
    if (found) { 
        auto formula_vector = facts.at(predicate);
        for (auto other_formula : formula_vector) {
            auto groundings_vector = other_formula->get_groundings();
            result = !groundings_vector.empty();
            for (auto grounding : groundings_vector) {
                grounding_table.add_grounding(std::move(grounding));
            }
        }
    }
    return result;
}

int Atom::get_variable_index(std::string variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}


} // namespace formula
} // namespace laser

