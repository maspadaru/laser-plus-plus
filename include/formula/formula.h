//
// Created by mike on 6/16/18.
//

#ifndef LASER_FORMULA_FORMULA_H
#define LASER_FORMULA_FORMULA_H

#include <string>

#include "formula/grounding_table.h"
#include "formula_type.h"

namespace laser {
namespace formula {

/**
 * Formula Interface
 */
class Formula {
private:
public:
// constructors / destructors

    virtual ~Formula() = default;

    virtual Formula &create() const = 0; // Virtual constructor (creation)
    virtual Formula &clone() const = 0;  // Virtual constructor (copying)

    /**
     * Virtual constructor (moving)
     *  All the contents of this object are moved to a new object allocated on
     *  the heap using 'new'.
     * @return reference to the newly allocated object.
     */
    virtual Formula &move() = 0;


// getters / setters

    virtual FormulaType get_type() const = 0;

    virtual std::vector<std::string> get_predicate_vector() const = 0;

// methods

    virtual std::vector<std::string> get_variable_names() const = 0;

    virtual int get_variable_index(std::string variable_name) const = 0;

    virtual bool is_satisfied() const = 0;

    virtual bool evaluate(
            uint64_t current_time,
            uint64_t current_tuple_counter,
            std::unordered_map<std::string, std::vector<formula::Formula *>>
            facts) = 0;

    virtual size_t get_number_of_variables() const = 0;

    virtual void expire_outdated_groundings(
            uint64_t current_time,
            uint64_t current_tuple_counter) = 0;

    virtual void add_grounding(
            uint64_t consideration_time, uint64_t horizon_time,
            uint64_t consideration_count, uint64_t horizon_count,
            std::vector<std::string> arguments) = 0;

    virtual void add_grounding(Grounding grounding) = 0;

    virtual std::vector<Grounding> get_groundings() = 0;

    virtual void debug_print() const = 0;
};


} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_H
