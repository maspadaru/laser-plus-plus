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
                     std::vector<std::shared_ptr<std::string const>> constant_vector)
    : consideration_time(consideration_time), horizon_time(horizon_time),
      consideration_count(consideration_count), horizon_count(horizon_count),
      is_fact_m(is_fact), is_background_fact_m(is_background_fact),
      constant_vector(std::move(constant_vector)) {
    init();
}

Grounding::Grounding(uint64_t consideration_time, uint64_t horizon_time,
                     uint64_t consideration_count, uint64_t horizon_count,
                     std::vector<std::shared_ptr<std::string const>> constant_vector)
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

size_t Grounding::get_substitution_hash() const {
    return substitution_hash;
}

size_t Grounding::get_full_hash() const { return full_hash; }

Grounding Grounding::new_annotations(uint64_t consideration_time,
                                     uint64_t horizon_time,
                                     uint64_t consideration_count,
                                     uint64_t horizon_count) const {
    Grounding result = Grounding(consideration_time, horizon_time,
                                 consideration_count, horizon_count, is_fact_m,
                                 is_background_fact_m, constant_vector);
    return result;
}

Grounding Grounding::new_horizon_time(uint64_t horizon_time) const {
    Grounding result = Grounding(consideration_time, horizon_time,
                                 consideration_count, horizon_count, is_fact_m,
                                 is_background_fact_m, constant_vector);
    return result;
}

void Grounding::init() {
    std::stringstream full_hash_stream;
    for (auto constant : constant_vector) {
        full_hash_stream << *constant << ";;";
    }
    std::string substitution_hash_str = full_hash_stream.str();
    full_hash_stream << consideration_time << ";;" << horizon_time << ";;"
                     << consideration_count << ";;" << horizon_count;
    std::string full_hash_str = full_hash_stream.str();
    size = constant_vector.size();
    std::hash<std::string> hasher;
    full_hash = hasher(full_hash_str);
    substitution_hash = hasher(substitution_hash_str);
}

std::string Grounding::get_constant(size_t variable_index) const {
    return *constant_vector.at(variable_index);
}

bool Grounding::has_expired(uint64_t time, uint64_t tuple_counter) const {
    // grounding expires when:
    // - At time point t, remove all groundings that expire at time t-1, so
    //   a grounding [1,1] will expire at time 2 : ht < time
    // - TODO hc <= tuple_counter?
    bool result = horizon_time < time || horizon_count <= tuple_counter;
    return result;
}

Grounding Grounding::new_constant(size_t index,
                                  std::string const &constant) const {
    std::vector<std::shared_ptr<std::string const>> new_vector = constant_vector;
    auto constant_ptr = std::make_shared<std::string>(constant);
    new_vector.insert(new_vector.begin() + index, constant_ptr);
    Grounding result =
        Grounding(consideration_time, horizon_time, consideration_count,
                  horizon_count, new_vector);
    return result;
}

Grounding Grounding::remove_constant(size_t index) const {
    std::vector<std::shared_ptr<std::string const>> new_vector = constant_vector;
    new_vector.erase(new_vector.begin() + index);
    Grounding result =
        Grounding(consideration_time, horizon_time, consideration_count,
                  horizon_count, new_vector);
    return result;
}

size_t Grounding::get_size() const { return size; }

std::string Grounding::debug_string() const {
    std::stringstream os;
    os << "Grounding -> size: " << get_size() << "; annotation vector: [ "
       << consideration_time << ", " << horizon_time << ", "
       << consideration_count << ", " << horizon_count << "] ";
    os << "; Constants: ";
    for (auto variable : constant_vector) {
        os << *variable << ", ";
    }
    return os.str();
}

bool Grounding::operator<(const Grounding &other) const {
    return full_hash < other.full_hash;
}

bool Grounding::operator==(const Grounding &other) const {
    return full_hash == other.full_hash;
}

bool Grounding::has_same_substitutions(const Grounding &other) const {
    return substitution_hash == other.substitution_hash;
}


//Grounding
//Grounding::new_constant_vector(std::vector<std::string> new_vector) const {
    //Grounding result =
        //Grounding(consideration_time, horizon_time, consideration_count,
                  //horizon_count, is_fact_m, is_background_fact_m, new_vector);
    //return result;
//}

} // namespace formula
} // namespace laser
