//
// Created by mike on 6/17/18.
//


#ifndef LASER_FORMULA_GROUNDING_H
#define LASER_FORMULA_GROUNDING_H

#include <climits>
#include <string>
#include <vector>

namespace laser {
namespace formula {

/**
 * Annotated Grounding (_alpha_-_sigma_)
 * (Definition 7)
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
    bool is_background_fact_m = false;
    // key: variable index; value: constant substitution_vector of key in this grounding
    // TODO try setting an initial size to avoid allocations
    std::vector<std::string> substitution_vector;
public:
// constructors & destructors

    Grounding(uint64_t consideration_time, uint64_t horizon_time,
              uint64_t consideration_count, uint64_t horizon_count);

    explicit Grounding(bool is_background_fact_m);

// getters & setters

    uint64_t get_consideration_time() const;

    uint64_t get_horizon_time() const;

    uint64_t get_consideration_count() const;

    uint64_t get_horizon_count() const;

    void set_horizon_time(uint64_t horizon_time);

// const methods

    std::string get_substitution(int variable_index) const;

// methods

    void add_substitution(int variable_index, std::string constant);

};


} // namespace formula
} // namespace laser


#endif // LASER_FORMULA_GROUNDING_H
