#ifndef LASER_FORMULA_EXTENDED_ATOM_H
#define LASER_FORMULA_EXTENDED_ATOM_H

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser::formula {

/**
 * Atomic formula
 */
class Atom : public Formula {
  private:
    FormulaType type = FormulaType::ATOM;
    GroundingTable grounding_table;
    bool is_head_m = false;
    bool has_duplicate_variables;
    std::vector<std::string> predicate_vector;
    std::map<std::string, size_t> arity_map;

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
    void accept(std::shared_ptr<util::Grounding> const &grounding);

    bool is_valid_fact(util::Grounding const &grounding) const;

  public:
    Atom() = default;

    explicit Atom(std::string predicate);

    explicit Atom(std::string predicate,
                  std::vector<std::string> variable_names);

    ~Atom() override = default;

    std::unique_ptr<formula::Formula> clone() const override;

    FormulaType get_type() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    size_t get_number_of_variables() const override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    void add_child(std::unique_ptr<formula::Formula> child) override;

    std::vector<std::unique_ptr<formula::Formula> const *>
    get_children() const override;

    uint64_t get_window_size() const override;
};

} // namespace laser::formula

#endif // LASER_FORMULA_EXTENDED_ATOM_H
