#ifndef LASER_FORMULA_FORMULA_TYPE_H
#define LASER_FORMULA_FORMULA_TYPE_H

namespace laser::formula {


enum class FormulaType {
    ATOM,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    NEGATION,
    TIME_REFERENCE,
    TUPLE_WINDOW,
    TIME_WINDOW,
    BOX,
    DIAMOND,
    EXISTENTIAL, 
    ALGEBRA,
    CONDITION,
    ASSIGNMENT
};


} // namespace laser::formula

#endif // LASER_FORMULA_FORMULA_TYPE_H
