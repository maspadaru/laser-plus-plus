//
// Created by mike on 6/17/18.
//

#include "util/grounding.h"

namespace laser {
namespace util {

// constructors & destructors

Grounding::Grounding(std::string predicate, uint64_t consideration_time,
                     uint64_t horizon_time, uint64_t consideration_count,
                     uint64_t horizon_count, bool is_fact,
                     bool is_background_fact,
                     std::vector<std::string> constant_vector)
    : predicate(std::move(predicate)), consideration_time(consideration_time),
      horizon_time(horizon_time), consideration_count(consideration_count),
      horizon_count(horizon_count), is_fact_m(is_fact),
      is_background_fact_m(is_background_fact),
      constant_vector(std::move(constant_vector)) {}

Grounding::Grounding(std::string predicate, uint64_t consideration_time,
                     uint64_t horizon_time, uint64_t consideration_count,
                     uint64_t horizon_count,
                     std::vector<std::string> constant_vector)
    : predicate(std::move(predicate)), consideration_time(consideration_time),
      horizon_time(horizon_time), consideration_count(consideration_count),
      horizon_count(horizon_count),
      constant_vector(std::move(constant_vector)) {}

size_t Grounding::get_step() const { return step; }

bool Grounding::is_fresh_sne(uint64_t now, size_t previous_step) const {
    // checking ct >= because facts can be derived at future timepoints from
    // TimeReference formulas in head of rules
    if (consideration_time > now) {
        return true;
    }
    // SNE Tier 1 -> considering only facts form current timepoint
    // SNE Tier 2 -> if the fact occurs at the curent timepoint, it should have
    // been derived sooner than the previous executin of the rule
    return consideration_time == now && step >= previous_step;
}

void Grounding::set_step(size_t step) { this->step = step; }

uint64_t Grounding::get_consideration_time() const {
    return consideration_time;
}

void Grounding::set_consideration_time(uint64_t ct) { consideration_time = ct; }

std::string const &Grounding::get_predicate() const { return predicate; }

uint64_t Grounding::get_horizon_time() const { return horizon_time; }

uint64_t Grounding::get_consideration_count() const {
    return consideration_count;
}

uint64_t Grounding::get_horizon_count() const { return horizon_count; }

bool Grounding::is_background_fact() const { return is_background_fact_m; }

bool Grounding::is_fact() const { return is_fact_m; }

size_t Grounding::get_hash() {
    if (!has_hash) {
        compute_hash();
        has_hash = true;
    }
    return hash;
}

std::shared_ptr<Grounding>
Grounding::new_annotations(uint64_t consideration_time, uint64_t horizon_time,
                           uint64_t consideration_count,
                           uint64_t horizon_count) const {
    auto result = Grounding(predicate, consideration_time, horizon_time,
                            consideration_count, horizon_count, is_fact_m,
                            is_background_fact_m, constant_vector);
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding> Grounding::clone() const {
    Grounding clone = Grounding(*this);
    return std::make_shared<Grounding>(clone);
}

std::shared_ptr<Grounding>
Grounding::new_horizon_time(uint64_t horizon_time) const {
    auto result = Grounding(predicate, consideration_time, horizon_time,
                            consideration_count, horizon_count, is_fact_m,
                            is_background_fact_m, constant_vector);
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding>
Grounding::new_horizon_count(uint64_t horizon_count) const {
    auto result = Grounding(predicate, consideration_time, horizon_time,
                            consideration_count, horizon_count, is_fact_m,
                            is_background_fact_m, constant_vector);
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding>
Grounding::new_constant_vector(std::vector<std::string> new_vector) const {
    Grounding result = Grounding(predicate, consideration_time, horizon_time,
                                 consideration_count, horizon_count, is_fact_m,
                                 is_background_fact_m, std::move(new_vector));
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding>
Grounding::new_pred_constvec(std::string const &predicate,
                             std::vector<std::string> new_vector) const {
    Grounding result = Grounding(predicate, consideration_time, horizon_time,
                                 consideration_count, horizon_count, is_fact_m,
                                 is_background_fact_m, std::move(new_vector));
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

void Grounding::compute_hash() {
    // old substitution_hash
    std::string hash_str = std::accumulate(
        constant_vector.begin(), constant_vector.end(), std::string(";"));
    std::hash<std::string> hasher;
    hash = hasher(hash_str);
}

std::string const &Grounding::get_constant(size_t variable_index) const {
    return constant_vector.at(variable_index);
}

std::vector<std::string> const &Grounding::get_constant_vector() const {
    return constant_vector;
}

bool Grounding::has_expired(uint64_t time, uint64_t tuple_counter) const {
    // grounding expires when:
    // - At time point t, remove all groundings that expire at time t-1, so
    //   a grounding [1,1] will expire at time 2 : ht < time
    // - TODO hc <= tuple_counter?
    bool result = horizon_time < time || horizon_count <= tuple_counter;
    return result;
}

std::shared_ptr<Grounding> Grounding::new_constant(size_t index,
                                                   std::string constant) const {
    std::vector<std::string> new_vector = constant_vector;
    new_vector.insert(new_vector.begin() + index, std::move(constant));
    Grounding result =
        Grounding(predicate, consideration_time, horizon_time,
                  consideration_count, horizon_count, std::move(new_vector));
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

std::shared_ptr<Grounding> Grounding::remove_constant(size_t index) const {
    std::vector<std::string> new_vector = constant_vector;
    new_vector.erase(new_vector.begin() + index);
    Grounding result =
        Grounding(predicate, consideration_time, horizon_time,
                  consideration_count, horizon_count, std::move(new_vector));
    result.step = this->step;
    return std::make_shared<Grounding>(result);
}

size_t Grounding::get_size() const { return constant_vector.size(); }

std::string Grounding::to_string() const {
    std::stringstream os;
    os << predicate << "(";
    for (auto const &value : constant_vector) {
        os << value << ", ";
    }
    os << " ) [ " << consideration_time << ", " << horizon_time << ", "
       << consideration_count << ", " << horizon_count << "] ";
    return os.str();
}

bool Grounding::annotation_less_than(Grounding const &other) const {

    // check each value. If they are equal, continue to the next value

    bool equal;
    bool less;

    equal = horizon_count == other.horizon_count;
    if (!equal) {
        less = horizon_count < other.horizon_count;
        return less;
    }

    equal = consideration_count == other.consideration_count;
    if (!equal) {
        less = consideration_count < other.consideration_count;
        return less;
    }

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

    return false; // *this == other
}

bool Grounding::predsubst_less_than(Grounding const &other) const {
    if (predicate < other.predicate) {
        return true;
    }
    if (predicate > other.predicate) {
        return false;
    }
    return substitution_less_than(other);
}

bool Grounding::substitution_less_than(Grounding const &other) const {
    // operator< for std::vector is said to have linear time complexity
    return constant_vector < other.constant_vector;
}

bool Grounding::operator<(Grounding const &other) const {
    if (annotation_less_than(other)) {
        return true;
    }
    if (other.annotation_less_than(*this)) {
        return false;
    }
    return substitution_less_than(other);
}

} // namespace util
} // namespace laser
