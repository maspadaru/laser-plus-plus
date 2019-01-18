//
// Created by mike on 6/17/18.
//

#include "formula/grounding.h"

namespace laser {
namespace formula {

// constructors & destructors

Grounding::Grounding(uint64_t consideration_time, uint64_t horizon_time,
                     uint64_t consideration_count, uint64_t horizon_count,
                     bool is_fact, bool is_background_fact,
                     std::vector<std::string> constant_vector)
    : consideration_time(consideration_time), horizon_time(horizon_time),
      consideration_count(consideration_count), horizon_count(horizon_count),
      is_fact_m(is_fact), is_background_fact_m(is_background_fact),
      constant_vector(std::move(constant_vector)) {
    init();
}

Grounding::Grounding(uint64_t consideration_time, uint64_t horizon_time,
                     uint64_t consideration_count, uint64_t horizon_count,
                     std::vector<std::string> constant_vector)
    : consideration_time(consideration_time), horizon_time(horizon_time),
      consideration_count(consideration_count), horizon_count(horizon_count),
      constant_vector(std::move(constant_vector)) {
    init();
}

uint64_t Grounding::get_consideration_time() const {
    return consideration_time;
}

uint64_t Grounding::get_horizon_time() const { return horizon_time; }

uint64_t Grounding::get_consideration_count() const {
    return consideration_count;
}

uint64_t Grounding::get_horizon_count() const { return horizon_count; }

bool Grounding::is_background_fact() const { return is_background_fact_m; }

bool Grounding::is_fact() const { return is_fact_m; }

size_t Grounding::get_substitution_hash() const { return substitution_hash; }

std::shared_ptr<Grounding>
Grounding::new_annotations(uint64_t consideration_time, uint64_t horizon_time,
                           uint64_t consideration_count,
                           uint64_t horizon_count) const {
    auto result = Grounding(consideration_time, horizon_time,
                            consideration_count, horizon_count, is_fact_m,
                            is_background_fact_m, constant_vector);
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding>
Grounding::new_horizon_time(uint64_t horizon_time) const {
    auto result = Grounding(consideration_time, horizon_time,
                            consideration_count, horizon_count, is_fact_m,
                            is_background_fact_m, constant_vector);
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding> Grounding::new_constant_vector(
    std::vector<std::string> const &new_vector) const {
    Grounding result =
        Grounding(consideration_time, horizon_time, consideration_count,
                  horizon_count, is_fact_m, is_background_fact_m, new_vector);
    return std::make_shared<Grounding>(result);
}

void Grounding::init() {
    std::string substitution_hash_str = std::accumulate(
        constant_vector.begin(), constant_vector.end(), std::string(";"));
    std::string full_hash_str =
        substitution_hash_str + std::to_string(consideration_time) +
        std::to_string(horizon_time) + std::to_string(consideration_count) +
        std::to_string(horizon_count);
    std::hash<std::string> hasher;
    full_hash = hasher(full_hash_str);
    substitution_hash = hasher(substitution_hash_str);
    size = constant_vector.size();
}

std::string Grounding::get_constant(size_t variable_index) const {
    return constant_vector.at(variable_index);
}

bool Grounding::has_expired(uint64_t time, uint64_t tuple_counter) const {
    // grounding expires when:
    // - At time point t, remove all groundings that expire at time t-1, so
    //   a grounding [1,1] will expire at time 2 : ht < time
    // - TODO hc <= tuple_counter?
    bool result = horizon_time < time || horizon_count <= tuple_counter;
    return result;
}

std::shared_ptr<Grounding>
Grounding::new_constant(size_t index, std::string const &constant) const {
    std::vector<std::string> new_vector = constant_vector;
    new_vector.insert(new_vector.begin() + index, constant);
    Grounding result =
        Grounding(consideration_time, horizon_time, consideration_count,
                  horizon_count, new_vector);
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding> Grounding::remove_constant(size_t index) const {
    std::vector<std::string> new_vector = constant_vector;
    new_vector.erase(new_vector.begin() + index);
    Grounding result =
        Grounding(consideration_time, horizon_time, consideration_count,
                  horizon_count, new_vector);
    return std::make_shared<Grounding>(result);
}

size_t Grounding::get_size() const { return size; }

std::string Grounding::debug_string() const {
    std::stringstream os;
    os << "Grounding -> size: " << get_size() << "; annotation vector: [ "
       << consideration_time << ", " << horizon_time << ", "
       << consideration_count << ", " << horizon_count << "] ";
    os << "; Constants: ";
    for (auto const &variable : constant_vector) {
        os << variable << ", ";
    }
    return os.str();
}

bool Grounding::annotation_less_than(Grounding const &other) const {

    // check each value. If they are equal, continue to the next value

    bool equal;
    bool less;

    equal = consideration_time == other.consideration_time;
    if (!equal) {
        less = consideration_time < other.consideration_time;
        return less;
    }

    equal = horizon_time == other.horizon_time;
    if (!equal) {
        less = horizon_time < other.horizon_time;
        return less;
    }

    equal = consideration_count == other.consideration_count;
    if (!equal) {
        less = consideration_count < other.consideration_count;
        return less;
    }

    equal = horizon_count == other.horizon_count;
    if (!equal) {
        less = horizon_count < other.horizon_count;
        return less;
    }

    return false; // *this == other
}

bool Grounding::substitution_less_than(Grounding const &other) const {
    // operator< for std::vector is said to have linear time complexity
    return constant_vector < other.constant_vector;
}

bool Grounding::operator<(Grounding const &other) const {
    // !p -> ( !q && (!q -> r) )
    bool p = annotation_less_than(other);
    if (p) return true;
    bool q = other.annotation_less_than(*this);
    if (q) return false;
    bool r = substitution_less_than(other);
    return r;
}

} // namespace formula
} // namespace laser
