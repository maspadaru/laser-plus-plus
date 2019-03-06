//
// Created by mike on 6/15/18.
//
#ifndef LASER_FORMULA_EXTENDED_ATOM_H
#define LASER_FORMULA_EXTENDED_ATOM_H

#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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
    bool is_head_m = false;
    bool has_duplicate_variables;

    /**
     * The names of all variables of the atom without any duplicates
     * E.g.: p(X, X, Y) -> ["X", "Y"]
     */
    std::vector<std::string> unique_variable_names;

    /**
     * Key: Variable Name
     * Value: Vector of positions where I can find an occurance of this bound
     * variable
     * E.g.: p(X, X, Y) -> { ("X":0), ("Y":2) }
     */
    std::unordered_map<std::string, std::vector<size_t>> binding_map;

    void set_variable_names(std::vector<std::string> &variable_names);

    void
    compute_unique_variable_names(std::vector<std::string> &variable_names);

    /**
     * If the grounding parameter is valid, it will be added to the atom's
     * Grounding Table
     */
    void accept(std::shared_ptr<Grounding> const &grounding);

    bool is_valid_fact(Grounding const &grounding) const;

    /** returns a vector containing only unique groundings in regards to
     * substitutions, not annotations
     */
    std::vector<std::shared_ptr<Grounding>> get_conclusion_set();

  public:
    // constructors & destructors

    Atom() = default;

    explicit Atom(std::string predicate);

    explicit Atom(std::string predicate,
                  std::vector<std::string> variable_names);

    ~Atom() override = default;

    Formula &create() const override;
    Formula &clone() const override;
    Formula &move() override;

    // getters & setters

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    // methods

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    bool
    evaluate(util::Timeline const &timeline,
             std::vector<std::shared_ptr<Grounding>> const &facts) override;

    size_t get_number_of_variables() const override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_ATOM_H
