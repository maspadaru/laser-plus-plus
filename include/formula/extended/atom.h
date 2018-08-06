//
// Created by mike on 6/15/18.
//
#ifndef LASER_FORMULA_EXTENDED_ATOM_H
#define LASER_FORMULA_EXTENDED_ATOM_H


#include <vector>
#include <string>
#include <iostream>

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
    GroundingTable grounding_table;
    std::vector<std::string> variable_names;


    /**
     *
     */
    std::vector<size_t> first_position_vector;

    /** 
     * Position -> position where I can find the first occurance of this bound 
     * variable
     */ 
    std::unordered_map<size_t, size_t> binding_map;
    
    void set_variable_names(std::vector<std::string> const& variable_names);

    void accept(Grounding const& grounding);

    bool is_valid_fact(Grounding const& grounding) const;

    Grounding remove_duplicate_variables(Grounding const& grounding); 

public:
// constructors & destructors

    Atom() = default;

    explicit Atom(std::string predicate);

    explicit Atom(std::string predicate,
                  std::vector<std::string> const& variable_names);

    ~Atom() override = default;

    Formula & create () const override;
    Formula & clone () const override;
    Formula & move () override;

// getters & setters

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

// methods

    std::vector<std::string> get_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    bool evaluate(
            uint64_t current_time,
            uint64_t current_tuple_counter,
            std::unordered_map<std::string, std::vector<formula::Grounding>>
            facts) override;

    size_t get_number_of_variables() const override;

    void expire_outdated_groundings(uint64_t current_time,
                                    uint64_t current_tuple_counter) override;


    void add_grounding(
            uint64_t consideration_time, uint64_t horizon_time,
            uint64_t consideration_count, uint64_t horizon_count,
            std::vector<std::string> arguments) override;

    void add_grounding(Grounding grounding) override;

    std::vector<Grounding> get_groundings() const override;

    void debug_print() const override;

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_ATOM_H
