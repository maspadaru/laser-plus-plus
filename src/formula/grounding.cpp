//
// Created by mike on 6/17/18.
//

#include <utility>
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

bool Grounding::is_background_fact() {
    return is_background_fact_m;
}

bool Grounding::is_fact() {
    return is_fact_m;
}

void Grounding::set_as_fact() {
    this->is_fact_m = true;
}

void Grounding::set_as_background_fact() {
    this->is_background_fact_m = true;
}
// methods

std::string Grounding::get_substitution(
        size_t variable_index) const {
    return substitution_vector.at(variable_index);
}


bool Grounding::has_expired(uint64_t time, uint64_t tuple_counter) const {
    // grounding expires when:
    // - At time point t, remove all groundings that expire at time t-1, so
    //   a grounding [1,1] will expire at time 2 : ht < time
    // - TODO hc <= tuple_counter?
    return horizon_time < time && horizon_count <= tuple_counter;
}

void Grounding::add_substitution(size_t variable_index,
                                 std::string constant) {
    if (substitution_vector.size() <= variable_index) {
        substitution_vector.resize(variable_index+5);
    }
    substitution_vector.at(variable_index) = std::move(constant);
}

void Grounding::add_substitution_vector(std::vector<std::string> vector) {
    substitution_vector.clear();
    substitution_vector.insert( substitution_vector.end(),
            vector.begin(), vector.end() );
}

size_t Grounding::get_size() const {
    return substitution_vector.size();
}

} // namespace formula
} // namespace laser
