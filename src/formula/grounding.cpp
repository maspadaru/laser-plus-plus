//
// Created by mike on 6/17/18.
//

#include "formula/grounding.h"

namespace laser {
namespace formula {

// constructors & destructors

Grounding::Grounding(long long int consideration_time,
                     long long int horizon_time,
                     long long int consideration_count,
                     long long int horizon_count) :
        consideration_time(consideration_time),
        horizon_time(horizon_time),
        consideration_count(consideration_count),
        horizon_count(horizon_count) {}

// getters & setters

long long int Grounding::get_consideration_time() const {
    return consideration_time;
}

long long int Grounding::get_horizon_time() const {
    return horizon_time;
}

long long int Grounding::get_consideration_count() const {
    return consideration_count;
}

long long int Grounding::get_horizon_count() const {
    return horizon_count;
}

void Grounding::set_horizon_time(long long int horizon_time) {
    Grounding::horizon_time = horizon_time;
}


// const methods

std::string const &Grounding::get_substitution(
        std::string const &variable) const {
    return substitution_map.at(variable);
}

// methods

void Grounding::add_substitution(std::string variable, std::string constant) {
    substitution_map.insert_or_assign(variable,constant);
}

} // namespace formula
} // namespace laser