//
// Created by mike on 6/15/18.
//
#ifndef LASER_FORMULA_EXTENDED_ATOM_H
#define LASER_FORMULA_EXTENDED_ATOM_H


#include <iostream> // DELETE
#include <vector>
#include <string>

#include <formula/formula.h>
#include <formula/groundingtable.h>

namespace laser {
namespace formula {

/**
 * Atomic formula
 */
class Atom : Formula {
private:
    FormulaType type = FormulaType::ATOM;
    std::string predicate = "Atom";
    std::vector<std::string> atom_arguments;
    bool _is_negated = false;
    GroundingTable grounding_table;
    GroundingTable gt_result;
public:
    Atom() {};
    ~Atom(){};
    Atom(std::string predicate);
    FormulaType get_type() {return this->type;}
    std::string get_predicate() {return predicate;}
    bool is_negated() {return _is_negated;}
    void print_me(); // DELETE
};


} // namespace formula
} // namespace laser



#endif //LASER_FORMULA_EXTENDED_ATOM_H
