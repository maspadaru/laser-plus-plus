#ifndef LASER_FORMULA_MATH_OPERATOR_H
#define LASER_FORMULA_MATH_OPERATOR_H

namespace laser::formula {


enum class MathOperator {
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    LESSER,
    GREATHER,
    EQUAL,
    LESSER_OR_EQUAL,
    GREATHER_OR_EQUAL,
    NOT_EQUAL
};


} // namespace laser::formula

#endif // LASER_FORMULA_MATH_OPERATOR_H
