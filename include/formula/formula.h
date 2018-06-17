//
// Created by mike on 6/16/18.
//

#ifndef LASERPP_FORMULA_FORMULA_H
#define LASERPP_FORMULA_FORMULA_H

#include <string>

#include <formula/groundingtable.h>

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


class Formula {
private:
public:
    virtual FormulaType get_type() = 0;
    virtual std::string get_predicate()  = 0;
    virtual bool is_negated() = 0;
    virtual void print_me() = 0; // DELETE
    virtual ~Formula(){}
};




} // namespace formula
} // namespace laser


#endif //LASERPP_FORMULA_FORMULA_H
