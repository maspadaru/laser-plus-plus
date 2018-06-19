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

// getters / setters

    virtual FormulaType get_type() const = 0;

    virtual std::string get_predicate() const = 0;

    virtual bool is_negated() const = 0;

    virtual bool had_input_already() const = 0;

// const methods

    virtual bool holds(long long int current_time) const = 0;

// methods

    virtual void accept(std::vector<Formula> facts, long long int current_time,
                        long long int current_tuple_counter) = 0;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_H
