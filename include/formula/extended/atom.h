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
    bool is_negated_m = false;
    bool had_input_already_m = false;
    std::vector<std::string> variable_names;
    GroundingTable grounding_table;
    GroundingTable result_grounding_table;
public:
// constructors & destructors

    explicit Atom(std::string predicate);

    explicit Atom(std::string predicate,
                  std::vector<std::string> variable_names);

    Atom() = default;

    ~Atom() override = default;

// getters & setters

    FormulaType get_type() const override;

    std::string get_predicate() const override;

    bool is_negated() const override;

    bool had_input_already() const override;

    std::vector<std::string> get_variable_names() const override;

// methods

    bool holds(long long int current_time) const override;

    size_t get_number_of_variables() const override;

    void accept(long long int current_time,
                long long int current_tuple_counter,
                std::vector<Formula *> facts) override;

    void expire_outdated_groundings(long long int current_time,
                                    long long int current_tuple_counter) override;

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_ATOM_H
