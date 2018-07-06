//
// Created by mike on 06.07.2018.
//

#ifndef LASER_FORMULA_PSEUDO_FORMULA_H
#define LASER_FORMULA_PSEUDO_FORMULA_H

#include <vector>
#include <string>

#include "formula_type.h"

namespace laser {
namespace formula {



struct PseudoFormula {
    PseudoFormula() = default;
    PseudoFormula(
            FormulaType type, std::string predicate,
            std::vector<std::string> arguments);

    // TODO needs to be extended for the other Formula types
    FormulaType type;
    std::string predicate;
    std::vector<std::string> arguments;

};

} // namespace formula
} // namespace laser


#endif // LASERPP_PSEUDO_FORMULA_H
