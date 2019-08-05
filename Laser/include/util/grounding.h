#ifndef LASER_UTIL_GROUNDING_H
#define LASER_UTIL_GROUNDING_H

#include <climits>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <functional>

namespace laser {
namespace util {

/**
 * Annotated Grounding (_alpha_-_sigma_)
 * Maps each variable in the formula _alpha_ to a constant value due to
 * substitution _sigma_
 * The grounding is annotated with consideration and horizon values
 */
class Grounding {

  private:
    uint64_t consideration_time = 0;
    uint64_t horizon_time = ULLONG_MAX;
    uint64_t consideration_count = 0;
    uint64_t horizon_count = ULLONG_MAX;
    size_t step = 0;
    bool is_background_fact_m = false;
    bool is_fact_m = false;
    std::vector<std::string> constant_vector;
    std::string predicate;

    bool annotation_less_than(Grounding const &other) const;

  public:
    Grounding(std::string predicate, uint64_t consideration_time,
              uint64_t horizon_time, uint64_t consideration_count,
              uint64_t horizon_count, std::vector<std::string> constant_vector);

    Grounding(std::string predicate, uint64_t consideration_time,
              uint64_t horizon_time, uint64_t consideration_count,
              uint64_t horizon_count, bool is_fact, bool is_background_fact,
              std::vector<std::string> constant_vector);

    Grounding(std::string predicate, uint64_t consideration_time,
                     uint64_t horizon_time, uint64_t consideration_count,
                     uint64_t horizon_count, bool is_fact,
                     bool is_background_fact, size_t vector_size);

    Grounding() = default;

    std::shared_ptr<Grounding> clone() const;

    /** creates a new grounding that shares the same annotations */
    std::shared_ptr<Grounding> empty_clone(std::string predicate, size_t vector_size) const;

    bool is_fresh_sne(uint64_t now, size_t previous_step) const;

    std::string const &get_predicate() const;

    uint64_t get_consideration_time() const;

    uint64_t get_horizon_time() const;

    uint64_t get_consideration_count() const;

    void set_consideration_time(uint64_t consideration_time);

    uint64_t get_horizon_count() const;

    size_t get_step() const;


    bool is_background_fact() const;

    bool is_fact() const;

    std::string const &get_constant(size_t variable_index) const;

    std::vector<std::string> const &get_constant_vector() const;

    std::string get_substitution_key() const;

    std::string get_annotation_key() const;

    bool has_expired(uint64_t time, uint64_t tuple_counter) const;

    size_t get_size() const;

    void set_step(size_t step);

    void set_constant(size_t index, std::string constant);

    void set_constant_vector(std::vector<std::string> &vector); 

    void clear_constant_vector(); 

    void push_constant(std::string const &constant); 

    void set_predicate(std::string const &predicate);

    void set_annotations(uint64_t consideration_time, uint64_t horizon_time,
                         uint64_t consideration_count, uint64_t horizon_count);

    void set_horizon_time(uint64_t horizon_time); 

    void set_horizon_count(uint64_t horizon_count);

    void remove_constant(size_t index);

    std::string to_string() const;

    std::string to_atom_string() const;

    bool operator<(const Grounding &other) const;

    bool operator==(const Grounding &other) const;

    bool substitution_less_than(Grounding const &other) const;
    bool predsubst_less_than(Grounding const &other) const;
};

struct GroundingFullHash {
public:
	size_t operator()(Grounding const &x) const {
        auto substitution_key = x.get_substitution_key();
        auto annotation_key = x.get_annotation_key();
        auto key_str = substitution_key + annotation_key;
        auto result = std::hash<std::string>{}(key_str);
        return result;
	}

	size_t operator()(std::shared_ptr<Grounding> const &x) const {
        auto substitution_key = x->get_substitution_key();
        auto annotation_key = x->get_annotation_key();
        auto key_str = substitution_key + annotation_key;
        auto result = std::hash<std::string>{}(key_str);
        return result;
	}
};

struct GroundingEqual {
    bool operator()(Grounding const &x, Grounding const &y) const {
        return x == y;
    }

    bool operator()(std::shared_ptr<Grounding> const &x,
                    std::shared_ptr<Grounding> const &y) const {
        return *x == *y;
    }
};

struct GroundingFullCompare {
    bool operator()(Grounding const &x, Grounding const &y) const {
        return x < y;
    }

    bool operator()(std::shared_ptr<Grounding> const &x,
                    std::shared_ptr<Grounding> const &y) const {
        return *x < *y;
    }
};

struct GroundingSubstitutionCompare {
    bool operator()(Grounding const &x, Grounding const &y) const {
        return x.substitution_less_than(y);
    }

    bool operator()(std::shared_ptr<Grounding> const &x,
                    std::shared_ptr<Grounding> const &y) const {
        return x->substitution_less_than(*y);
    }
};

struct GroundingPredicateSubstitutionCompare {
    bool operator()(Grounding const &x, Grounding const &y) const {
        return x.predsubst_less_than(y);
    }

    bool operator()(std::shared_ptr<Grounding> const &x,
                    std::shared_ptr<Grounding> const &y) const {
        return x->predsubst_less_than(*y);
    }
};

} // namespace util
} // namespace laser

#endif // LASER_UTIL_GROUNDING_H
