//
// Created by mike on 06.07.2018.
//

#ifndef LASER_FORMULA_FORMULA_TYPE_H
#define LASER_FORMULA_FORMULA_TYPE_H



namespace laser {
namespace formula {


enum class FormulaType {
    ATOM,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    NEGATION,
    EXACT_TIME,
    TUPLE_WINDOW,
    TIME_WINDOW,
    BOX,
    DIAMOND
};


} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_TYPE_H
