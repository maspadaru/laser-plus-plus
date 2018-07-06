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

    virtual std::string get_predicate() const = 0;

    virtual bool is_negated() const = 0;

    virtual bool had_input_already() const = 0;

    virtual std::vector<std::string> get_variable_names() const = 0;

// methods

    virtual bool holds(uint64_t current_time) const = 0;

    virtual size_t get_number_of_variables() const = 0;

    virtual void accept(
            uint64_t current_time,
            uint64_t current_tuple_counter,
            std::vector<Formula *> facts) = 0;

    virtual void expire_outdated_groundings(
            uint64_t current_time,
            uint64_t current_tuple_counter) = 0;

    virtual void add_grounding(
            uint64_t consideration_time, uint64_t horizon_time,
            uint64_t consideration_count, uint64_t horizon_count,
            std::vector<std::string> arguments) = 0;

    // TODO Formula should be final version before starting to implement other
    // TODO formula classes. Thus: implement working program for Atoms first
    // Other potentially useful functions
    // get all substitutionos of variable name
    // get all substitutionos of variable index
    // get map variable name to index
    // copy groundings from formula with same predicate

    virtual void debug_print() const = 0;
};


} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_H
