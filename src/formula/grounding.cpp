//
// Created by mike on 6/17/18.
//

#include "formula/grounding.h"

namespace laser {
namespace formula {

// constructors & destructors

Grounding::Grounding(uint64_t consideration_time,
                     uint64_t horizon_time,
                     uint64_t consideration_count,
                     uint64_t horizon_count) :
        consideration_time(consideration_time),
        horizon_time(horizon_time),
        consideration_count(consideration_count),
        horizon_count(horizon_count) {}

Grounding::Grounding(bool is_background_fact_m) {
    this->is_background_fact_m = is_background_fact_m;
}

// getters & setters

uint64_t Grounding::get_consideration_time() const {
    return consideration_time;
}

uint64_t Grounding::get_horizon_time() const {
    return horizon_time;
}

uint64_t Grounding::get_consideration_count() const {
    return consideration_count;
}

uint64_t Grounding::get_horizon_count() const {
    return horizon_count;
}

void Grounding::set_horizon_time(uint64_t horizon_time) {
    Grounding::horizon_time = horizon_time;
}


// const methods

std::string Grounding::get_substitution(
        int variable_index) const {
    return substitution_vector.at(variable_index);
}

// methods

void Grounding::add_substitution(int variable_index,
                                 std::string constant) {
    if (substitution_vector.size() <= variable_index) {
        substitution_vector.resize(variable_index+5);
    }
    substitution_vector.at(variable_index) = constant;
}

} // namespace formula
} // namespace laser