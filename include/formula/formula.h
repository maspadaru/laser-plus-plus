//
// Created by mike on 6/16/18.
//

#ifndef LASER_FORMULA_FORMULA_H
#define LASER_FORMULA_FORMULA_H

#include <string>

#include "formula/grounding_table.h"

namespace laser {
namespace formula {

enum class FormulaType {
    MATH,
    COMP,
    ATOM,
    NEGATED_ATOM,
    AT_ATOM,
    AT_NEGATED_ATOM,
    AT_VAR_ATOM,
    AT_VAR_NEGATED_ATOM,
};

/**
 * Formula Interface
 */
class Formula {
private:
public:
// constructors / destructors

    virtual ~Formula() = default;
    virtual Formula * create () const = 0; // Virtual constructor (creation)
    virtual Formula * clone () const = 0;  // Virtual constructor (copying)


// getters / setters

    virtual FormulaType get_type() const = 0;

    virtual std::string get_predicate() const = 0;

    virtual bool is_negated() const = 0;

    virtual bool had_input_already() const = 0;

    virtual std::vector<std::string> get_variable_names() const = 0;

// methods

    virtual bool holds(unsigned long long int current_time) const = 0;

    virtual size_t get_number_of_variables() const = 0;

    virtual void accept(unsigned long long int current_time,
                        unsigned long long int current_tuple_counter,
                        std::vector<Formula *> facts) = 0;

    virtual void expire_outdated_groundings(unsigned long long int current_time,
                                            unsigned long long int current_tuple_counter) = 0;

    // TODO Formula should be final version before starting to implement other
    // TODO formula classes. Thus: implement working program for Atoms first
    // Other potentially useful functions
    // get all substitutionos of variable name
    // get all substitutionos of variable index
    // get map variable name to index
    // copy groundings from formula with same predicate
};


} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_H
