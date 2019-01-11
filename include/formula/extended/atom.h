//
// Created by mike on 6/15/18.
//
#ifndef LASER_FORMULA_EXTENDED_ATOM_H
#define LASER_FORMULA_EXTENDED_ATOM_H

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

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

    /**
     * The names of all arguments of the atom
     * E.g.: p(X, X, Y) -> ["X", "X", "Y"]
     */
    std::vector<std::string> full_variable_names;

    /**
     * If a variable occurs in the atom multiple times, this vector will only
     * contain the position of the first occurance of each varaible.
     * E.g.: p(X, X, Y) -> [0, 2]
     */
    std::vector<size_t> first_position_vector;

    /**
     * Position -> position where I can find the first occurance of this bound
     * variable
     * E.g.: p(X, X, Y) -> { ("X":0), ("Y":2) }
     */
    std::unordered_map<size_t, size_t> binding_map;

    /**
     * Extracts the unique variable names from the list of atom arguments
     * and passes them to the Grounding Table
     */
    void set_variable_names(std::vector<std::string> const &variable_names);

    /**
     * If the grounding parameter is valid, it will be added to the atom's
     * Grounding Table
     */
    void accept(std::shared_ptr<Grounding> const &grounding);

    bool is_valid_fact(Grounding const &grounding) const;

    /**
     * A valid grounding is processed to fit in the atom's Grounding Table
     * E.g: Atom = p(x, x, y); with Grounding Table = ["X", "Y"]
     * Grounding = ["x1", "x1", "y1"] -> Grounding = ["x1", "y1"]
     */
    std::shared_ptr<Grounding> remove_duplicate_variables(Grounding const &grounding);

    template <typename T>
    void debug_print(std::string const &message, T const &value) const;

    /** returns a vector containing only unique groundings in regards to
     * substitutions, not annotations
     */
    std::vector<std::shared_ptr<Grounding>> get_conclusion_set();

  public:
    // constructors & destructors

    Atom() = default;

    explicit Atom(std::string predicate);

    explicit Atom(std::string predicate,
                  std::vector<std::string> const &variable_names);

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

    std::vector<std::string> get_variable_names() const override;

    std::vector<std::string> get_full_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    bool evaluate(
        util::Timeline timeline,
        std::unordered_map<
            std::string, std::vector<std::shared_ptr<Grounding>>> const
            &facts) override;

    size_t get_number_of_variables() const override;

    void expire_outdated_groundings(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>> get_groundings(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_timepoint(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_step(util::Timeline timeline) override;

    void add_child(formula::Formula *child) override;

    std::string debug_string() const override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_ATOM_H
