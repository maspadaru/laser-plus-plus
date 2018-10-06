//
// Created by mike on 6/17/18.
//

#ifndef LASER_FORMULA_GROUNDING_H
#define LASER_FORMULA_GROUNDING_H

#include <climits>
#include <string>
#include <sstream>
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
    bool is_fact_m = false;
    // TODO try setting an initial size to avoid allocations
    std::vector<std::string> substitution_vector;

  public:
    // constructors & destructors

    Grounding(uint64_t consideration_time, uint64_t horizon_time,
              uint64_t consideration_count, uint64_t horizon_count);

    Grounding() = default;

    // getters & setters

    uint64_t get_consideration_time() const;

    uint64_t get_horizon_time() const;

    uint64_t get_consideration_count() const;

    uint64_t get_horizon_count() const;

    void set_horizon_time(uint64_t horizon_time);

    void set_consideration_time(uint64_t horizon_time);

    bool is_background_fact();

    bool is_fact();

    void set_as_fact();

    void set_as_background_fact();

    // methods

    std::string get_substitution(size_t variable_index) const;

    /** Creates a string witch is the same for all groundings that 
     * share the same constants but with different annotations.
     * It is used where a grounding needs to be a key of a map, for instance,
     * in Box::get_groundings() / Box::build_maps(). 
     * TODO: This should be replaced with a hash function for efficiency later
     * and the resulting hash should be used as the key
     */
    std::string compute_hash() const;

    void add_substitution(size_t variable_index, std::string constant);

    void add_substitution_vector(std::vector<std::string> vector);

    bool has_expired(uint64_t time, uint64_t tuple_counter) const;

    size_t get_size() const;

    std::string debug_string() const;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_GROUNDING_H
