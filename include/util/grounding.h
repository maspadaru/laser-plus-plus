#ifndef LASER_UTIL_GROUNDING_H
#define LASER_UTIL_GROUNDING_H

#include <climits>
#include <functional> // std::hash<std::string>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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

    bool has_hash = false;
    size_t hash = 0;

    void compute_hash();
    bool annotation_less_than(Grounding const &other) const;

  public:
    Grounding(std::string predicate, uint64_t consideration_time,
              uint64_t horizon_time, uint64_t consideration_count,
              uint64_t horizon_count, std::vector<std::string> constant_vector);

    Grounding(std::string predicate, uint64_t consideration_time,
              uint64_t horizon_time, uint64_t consideration_count,
              uint64_t horizon_count, bool is_fact, bool is_background_fact,
              std::vector<std::string> constant_vector);

    Grounding() = default;

    std::shared_ptr<Grounding> clone() const;

    bool is_fresh_sne(uint64_t now, size_t previous_step) const;

    std::string const &get_predicate() const;

    uint64_t get_consideration_time() const;

    uint64_t get_horizon_time() const;

    uint64_t get_consideration_count() const;

    void set_consideration_time(uint64_t consideration_time);

    uint64_t get_horizon_count() const;

    size_t get_step() const;

    void set_step(size_t step);

    bool is_background_fact() const;

    bool is_fact() const;

    size_t get_hash();

    std::string const &get_constant(size_t variable_index) const;

    std::vector<std::string> const &get_constant_vector() const;

    std::string get_substitution_key() const;

    bool has_expired(uint64_t time, uint64_t tuple_counter) const;

    size_t get_size() const;

    /**
     * Creates a new Grounding containing an extra constant at the specified
     * index
     */
    std::shared_ptr<Grounding> new_constant(size_t index,
                                            std::string constant) const;

    /**
     * Creates a new Grounding containing the new constant vector
     */
    std::shared_ptr<Grounding>
    new_constant_vector(std::vector<std::string> new_vector) const;

    /**
     * Creates a new Grounding containing the new predicate and constant vector
     */
    std::shared_ptr<Grounding>
    new_pred_constvec(std::string const &predicate,
                      std::vector<std::string> new_vector) const;

    /**
     * Creates a new Grounding containing the new annotations
     */
    std::shared_ptr<Grounding> new_annotations(uint64_t consideration_time,
                                               uint64_t horizon_time,
                                               uint64_t consideration_count,
                                               uint64_t horizon_count) const;

    /**
     * Creates a new Grounding containing the new horizon time
     */
    std::shared_ptr<Grounding> new_horizon_time(uint64_t horizon_time) const;

    /**
     * Creates a new Grounding containing the new horizon tuple count
     */
    std::shared_ptr<Grounding> new_horizon_count(uint64_t horizon_count) const;

    /**
     * Creates a new Grounding containing that contains all constants, except
     * the one at the specified index
     */
    std::shared_ptr<Grounding> remove_constant(size_t index) const;

    std::string to_string() const;

    bool operator<(const Grounding &other) const;

    bool substitution_less_than(Grounding const &other) const;
    bool predsubst_less_than(Grounding const &other) const;
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
