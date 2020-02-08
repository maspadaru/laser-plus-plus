#ifndef LASER_FORMULA_MATH_OPERATOR_H
#define LASER_FORMULA_MATH_OPERATOR_H

namespace laser::formula {


enum class MathOperator {
    EQUALS,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    LESSER,
    GREATHER,
    LESSER_OR_EQUAL,
    GREATHER_OR_EQUAL,
    NOT_EQUAL,
    NONE
};


} // namespace laser::formula

#endif // LASER_FORMULA_MATH_OPERATOR_H
