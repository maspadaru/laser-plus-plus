#include "util/grounding.h"

namespace laser::util {

Grounding::Grounding(std::string const &predicate, uint64_t consideration_time,
                     uint64_t horizon_time, uint64_t consideration_count,
                     uint64_t horizon_count,
                     std::vector<std::string> const &constant_vector)
    : consideration_time(consideration_time), horizon_time(horizon_time),
      consideration_count(consideration_count), horizon_count(horizon_count) {
    init_strings(predicate, constant_vector);
}

Grounding::Grounding(std::string const &predicate, uint64_t consideration_time,
                     uint64_t horizon_time, uint64_t consideration_count,
                     uint64_t horizon_count, bool is_fact,
                     bool is_background_fact,
                     std::vector<std::string> const &constant_vector)
    : consideration_time(consideration_time), horizon_time(horizon_time),
      consideration_count(consideration_count), horizon_count(horizon_count),
      is_fact_m(is_fact), is_background_fact_m(is_background_fact) {
    init_strings(predicate, constant_vector);
}

// this constructor is used in Grounding::empty_clone
Grounding::Grounding(std::string const &predicate, uint64_t consideration_time,
                     uint64_t horizon_time, uint64_t consideration_count,
                     uint64_t horizon_count, bool is_fact,
                     bool is_background_fact, size_t vector_size)
    : consideration_time(consideration_time), horizon_time(horizon_time),
      consideration_count(consideration_count), horizon_count(horizon_count),
      is_fact_m(is_fact), is_background_fact_m(is_background_fact) {
    auto &string_pool = StringPool::get_instance();
    this->predicate = string_pool.add_string(predicate);
    constant_vector.reserve(vector_size);
}

void Grounding::init_strings(std::string const &predicate,
                             std::vector<std::string> const &constant_vector) {
    auto &string_pool = StringPool::get_instance();
    this->predicate = string_pool.add_string(predicate);
    for (auto const &str : constant_vector) {
        auto index = string_pool.add_string(str);
        this->constant_vector.push_back(index);
    }
}

std::string const &Grounding::get_string(size_t index) const {
    auto &string_pool = StringPool::get_instance();
    return string_pool.get_string(index);
}

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

uint64_t Grounding::get_horizon_time() const { return horizon_time; }

uint64_t Grounding::get_consideration_count() const {
    return consideration_count;
}

uint64_t Grounding::get_horizon_count() const { return horizon_count; }

std::string const &Grounding::get_predicate() const {
    return get_string(predicate);
}

bool Grounding::is_background_fact() const { return is_background_fact_m; }

bool Grounding::is_fact() const { return is_fact_m; }

void Grounding::set_annotations(uint64_t consideration_time,
                                uint64_t horizon_time,
                                uint64_t consideration_count,
                                uint64_t horizon_count) {
    this->consideration_time = consideration_time;
    this->horizon_time = horizon_time;
    this->consideration_count = consideration_count;
    this->horizon_count = horizon_count;
}

void Grounding::set_horizon_time(uint64_t horizon_time) {
    this->horizon_time = horizon_time;
}

void Grounding::set_horizon_count(uint64_t horizon_count) {
    this->horizon_count = horizon_count;
}

std::shared_ptr<Grounding> Grounding::deep_clone() const {
    // There is no difference between deep and shallow clone anymore.
    // There is no point in having a deep clone while using a global string pool
    auto result = std::make_shared<Grounding>(*this);
    // result->set_constant_vector(*constant_vector);
    return result;
}

std::shared_ptr<Grounding> Grounding::shallow_clone() const {
    return std::make_shared<Grounding>(*this);
}

std::shared_ptr<Grounding> Grounding::empty_clone(std::string const &predicate,
                                                  size_t vector_size) const {
    return std::make_shared<Grounding>(
        predicate, this->consideration_time, this->horizon_time,
        this->consideration_count, this->horizon_count, this->is_fact_m,
        this->is_background_fact_m, vector_size);
}

void Grounding::set_constant_vector(std::vector<std::string> const &vector) {
    auto &string_pool = StringPool::get_instance();
    for (auto const &str : vector) {
        auto index = string_pool.add_string(str);
        this->constant_vector.push_back(index);
    }
}

void Grounding::clear_constant_vector() { constant_vector.clear(); }

void Grounding::push_constant(std::string const &constant) {
    auto &string_pool = StringPool::get_instance();
    constant_vector.push_back(string_pool.add_string(constant));
}

void Grounding::set_predicate(std::string const &predicate) {
    auto &string_pool = StringPool::get_instance();
    this->predicate = string_pool.add_string(predicate);
}

std::string const &Grounding::get_constant(size_t variable_index) const {
    auto &string_pool = StringPool::get_instance();
    auto pool_index = constant_vector.at(variable_index);
    return get_string(pool_index);
}

std::vector<std::string> Grounding::get_constant_vector() const {
    std::vector<std::string> result;
    auto &string_pool = StringPool::get_instance();
    for (auto index : constant_vector) {
        auto const &str = string_pool.get_string(index);
        result.push_back(str);
    }
    return result;
}

bool Grounding::has_expired(uint64_t time, uint64_t tuple_counter) const {
    // grounding expires when:
    // - At time point t, remove all groundings that expire at time t-1, so
    //   a grounding [1,1] will expire at time 2 : ht < time
    // - TODO hc <= tuple_counter?
    bool result = horizon_time < time || horizon_count <= tuple_counter;
    return result;
}

void Grounding::set_constant(size_t index, std::string constant) {
    auto &string_pool = StringPool::get_instance();
    constant_vector.insert(constant_vector.begin() + index,
                            string_pool.add_string(constant));
}

void Grounding::remove_constant(size_t index) {
    constant_vector.erase(constant_vector.begin() + index);
}

size_t Grounding::get_size() const { return constant_vector.size(); }

std::string Grounding::get_substitution_key() const {
    // This is used in box so I need to have no conflicts
    auto vect = this->get_constant_vector();
    std::string hash_str = std::accumulate(
        vect.begin(), vect.end(), std::string(";"));
    return hash_str;
}

std::string Grounding::get_annotation_key() const {
    auto result =
        std::to_string(consideration_time) + std::to_string(horizon_time) +
        std::to_string(consideration_count) + std::to_string(horizon_count);
    return result;
}

std::string Grounding::to_atom_string() const {
    std::stringstream os;
    os << get_string(predicate) << "(";
    for (auto value : constant_vector) {
        os << get_string(value) << ", ";
    }
    os << ")";
    return os.str();
}

std::string Grounding::to_string() const {
    std::stringstream os;
    os << get_string(predicate) << "(";
    for (auto const &value : constant_vector) {
        os << get_string(value) << ", ";
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
    if (predicate < (other.predicate)) {
        return true;
    }
    if (predicate > (other.predicate)) {
        return false;
    }
    return substitution_less_than(other);
}

bool Grounding::substitution_less_than(Grounding const &other) const {
    // operator< for std::vector is said to have linear time complexity
    return constant_vector < (other.constant_vector);
}

bool Grounding::substitution_equal(Grounding const &other) const {
    return constant_vector == (other.constant_vector);
}

bool Grounding::operator==(Grounding const &other) const {
    bool result;
    result = this->constant_vector == other.constant_vector &&
             this->consideration_time == other.consideration_time &&
             this->horizon_time == other.horizon_time &&
             this->consideration_count == other.consideration_count &&
             this->horizon_count == other.horizon_count;
    return result;
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

} // namespace laser::util
