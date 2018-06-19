//
// Created by mike on 6/15/18.
//
#ifndef LASER_FORMULA_EXTENDED_ATOM_H
#define LASER_FORMULA_EXTENDED_ATOM_H


#include <vector>
#include <string>

#include "formula/formula.h"
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

/**
 * Atomic formula
 */
class Atom : public Formula {
private:
    FormulaType type = FormulaType::ATOM;
    std::string predicate = "Atom";
    std::vector<std::string> atom_arguments;
    bool is_negated_m = false;
    bool had_input_already_m = false;
    GroundingTable grounding_table;
    GroundingTable result_grounding_table;
public:
// constructors & destructors

    explicit Atom(std::string predicate);

    Atom() = default;

    ~Atom() override = default;

// getters & setters

    FormulaType get_type() const override;

    std::string get_predicate() const override;

    bool is_negated() const override;

    bool had_input_already() const override;

// const methods

    bool holds(long long int current_time) const override;

// methods

    void accept(std::vector<Formula> facts, long long int current_time,
                long long int current_tuple_counter) override;

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_ATOM_H
