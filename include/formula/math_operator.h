#ifndef LASER_FORMULA_MATH_OPERATOR_H
#define LASER_FORMULA_MATH_OPERATOR_H

namespace laser::formula {

enum class MathOperator {
    ASSIGNMENT,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    EQUALS,
    NOT_EQUAL,
    LESSER,
    GREATHER,
    LESSER_OR_EQUAL,
    GREATHER_OR_EQUAL,
    NONE
};

} // namespace laser::formula

#endif // LASER_FORMULA_MATH_OPERATOR_H
