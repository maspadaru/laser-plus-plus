//
// Created by mike on 06.07.2018.
//

#ifndef LASER_FORMULA_FORMULA_TYPE_H
#define LASER_FORMULA_FORMULA_TYPE_H



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


} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_TYPE_H
