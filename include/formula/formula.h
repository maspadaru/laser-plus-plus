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
    virtual FormulaType get_type() = 0;
    virtual std::string get_predicate()  = 0;
    virtual bool is_negated() = 0;
    virtual ~Formula(){}
};




} // namespace formula
} // namespace laser


#endif // LASER_FORMULA_FORMULA_H
