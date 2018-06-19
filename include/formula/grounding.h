//
// Created by mike on 6/17/18.
//


#ifndef LASER_FORMULA_GROUNDING_H
#define LASER_FORMULA_GROUNDING_H

#include <climits>
#include <string>
#include <unordered_map>

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
    long long int consideration_time = 0;
    long long int horizon_time = ULLONG_MAX;
    long long int consideration_count = 0;
    long long int horizon_count = ULLONG_MAX;
    // key: variable; value: constant substitution_map of key in this grounding
    std::unordered_map<std::string, std::string> substitution_map;
public:
// constructors & destructors

    Grounding(long long int consideration_time, long long int horizon_time,
              long long int consideration_count, long long int horizon_count);

// getters & setters

    long long int get_consideration_time() const;

    long long int get_horizon_time() const;

    long long int get_consideration_count() const;

    long long int get_horizon_count() const;

    void set_horizon_time(long long int horizon_time);

// const methods

    std::string const &get_substitution(std::string const &variable) const;

// methods

    void add_substitution(std::string variable, std::string constant);

};


} // namespace formula
} // namespace laser


#endif // LASER_FORMULA_GROUNDING_H
