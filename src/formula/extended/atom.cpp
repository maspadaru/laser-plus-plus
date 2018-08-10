//
// Created by mike on 6/15/18.
//

#include "formula/extended/atom.h"

namespace laser {
namespace formula {

// constructors & destructors

Atom::Atom(std::string predicate) { this->predicate = std::move(predicate); }

Atom::Atom(std::string predicate,
           std::vector<std::string> const &variable_names) {
    this->predicate = std::move(predicate);
    set_variable_names(variable_names);
    debug_print("Constructed ATOM:", this->debug_string());
}

void Atom::set_variable_names(std::vector<std::string> const &variable_names) {
    this->full_variable_names = variable_names;
    std::vector<std::string> result;
    std::unordered_map<std::string, int> temp;
    for (size_t index = 0; index < variable_names.size(); index++) {
        std::string name = variable_names.at(index);
        temp.try_emplace(name, -1);
        if (temp.at(name) >= 0) {
            debug_print("Variable Name ", name);
            debug_print("inserting in binding map, index: ", index);
            debug_print("inserting in binding map, temp: ", temp.at(name));
            binding_map.insert({index, temp.at(name)});
        } else {
            first_position_vector.push_back(index);
            result.push_back(name);
            temp.at(name) = index;
        }
    }
    grounding_table.set_variable_names(result);
}

Formula &Atom::create() const {
    auto result = new Atom();
    return *result;
}

Formula &Atom::clone() const {
    auto result = new Atom(*this);
    // TODO ? Is this enough?
    return *result;
}

Formula &Atom::move() {
    Atom *result = new Atom(std::move(this->predicate));
    result->type = this->type;
    result->full_variable_names = std::move(this->full_variable_names);
    result->binding_map = std::move(this->binding_map);
    result->first_position_vector = std::move(this->first_position_vector);
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


std::vector<std::string> Atom::get_full_variable_names() const {
    return full_variable_names;
} 

// methods

size_t Atom::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

void Atom::expire_outdated_groundings(uint64_t current_time,
                                      uint64_t current_tuple_counter) {
    grounding_table.expire_outdated_groundings(current_time,
                                               current_tuple_counter);
}

std::string Atom::debug_string() const {
    std::stringstream os;
    os << "ATOM -> predicate:  " << this->predicate;
    os << "; variable_names.size=: " << this->get_number_of_variables()
       << "; Names: ";
    for (auto const &variable : this->get_variable_names()) {
        os << variable << ", ";
    }
    os << "GroundingTable->size:" << grounding_table.get_size();
    return os.str();
}

void Atom::add_grounding(uint64_t consideration_time, uint64_t horizon_time,
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

std::vector<Grounding> Atom::get_groundings() const {
    return grounding_table.get_all_groundings();
}

bool Atom::is_satisfied() const { return grounding_table.get_size() > 0; }

bool Atom::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    if (facts.count(predicate) > 0) {
        auto grounding_vector = facts.at(predicate);
        for (auto const &grounding : grounding_vector) {
            debug_print("evaluate -> current_time", current_time);
            debug_print("evaluate -> atom: ", this->debug_string());
            debug_print("evaluate -> Grounding: ", grounding.debug_string());
            accept(grounding);
            debug_print("evaluate -> after accept, grouding table size", 
                    this->grounding_table.get_size());
        }
    }
    return this->is_satisfied();
}

void Atom::accept(Grounding const &grounding) {
    auto grounding_size = grounding.get_size();
    auto atom_size = this->get_number_of_variables();
    if (atom_size == grounding_size) {
        grounding_table.add_grounding(grounding);
    } else if (atom_size <= grounding_size && is_valid_fact(grounding)) {
        auto valid_grounding = remove_duplicate_variables(grounding);
        grounding_table.add_grounding(valid_grounding);
    } else {
        // TODO some sort of error
    }
}

int Atom::get_variable_index(std::string variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

bool Atom::is_valid_fact(Grounding const &grounding) const {
    debug_print("is_valid_fact()", "start");
    bool is_valid = true;
    for (auto const &iterator : binding_map) {
        debug_print("iterator first", iterator.first);
        debug_print("iterator second", iterator.second);
        auto first = grounding.get_substitution(iterator.first);
        auto second = grounding.get_substitution(iterator.second);
        is_valid &= first == second;
    }
    debug_print("is_valid_fact()", "end");
    return is_valid;
}

Grounding Atom::remove_duplicate_variables(Grounding const &grounding) {
    debug_print("remove_duplicate_groundings()", "start");
    Grounding result = Grounding(
        grounding.get_consideration_time(), grounding.get_horizon_time(),
        grounding.get_consideration_count(), grounding.get_horizon_count());
    std::vector<std::string> result_values;
    for (size_t index : first_position_vector) {
        result_values.push_back(grounding.get_substitution(index));
    }
    result.add_substitution_vector(result_values);
    debug_print("remove_duplicate_groundings()", "end");
    return result;
}

template <typename T>
void Atom::debug_print(std::string const &message, T const &value) const {
    std::cerr << "ATOM -> predicate: " << this->predicate << " -> ";
    std::cerr << message << " : " << value << std::endl;
}

} // namespace formula
} // namespace laser
