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

    Atom() = default;

    /* Will be implicitly-defined by the compiler.
     * Since the class is NOT managing a resource whose handle is an object of
     * a non-class type (raw pointer, POSIX file descriptor, etc), this
     * implicitly-defined copy constructor should be good enough
     */
    // Atom(Atom const& atom); // Copy constructor

    explicit Atom(std::string predicate);

    explicit Atom(std::string predicate,
                  std::vector<std::string> variable_names);

    ~Atom() override = default;

    Atom & create () const override;
    Atom & clone () const override;
    Atom & move () override;

// getters & setters

    FormulaType get_type() const override;

    std::string get_predicate() const override;

    bool is_negated() const override;

    bool had_input_already() const override;

    std::vector<std::string> get_variable_names() const override;

// methods

    bool holds(unsigned long long int current_time) const override;

    size_t get_number_of_variables() const override;

    void accept(unsigned long long int current_time,
                unsigned long long int current_tuple_counter,
                std::vector<Formula *> facts) override;

    void expire_outdated_groundings(unsigned long long int current_time,
                                    unsigned long long int current_tuple_counter) override;

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_ATOM_H
