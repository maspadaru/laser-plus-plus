//
// Created by mike on 06.07.2018.
//

#include "formula/pseudo_formula.h"

namespace laser {
namespace formula {


PseudoFormula::PseudoFormula(
        laser::formula::FormulaType type, std::string predicate,
        std::vector<std::string> arguments)
        : type(type), predicate(std::move(predicate)), arguments(
        std::move(arguments)) {}



} // namespace formula
} // namespace laser