#ifndef LASER_RULE_RULE_H
#define LASER_RULE_RULE_H

#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
#include <utility>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "rule/chase_filter.h"
#include "rule/filter/oblivious_filter.h"
#include "rule/filter/restricted_filter.h"
#include "rule/filter/skolem_filter.h"
#include "rule/math_atom.h"
#include "rule/shared.h"
#include "util/chase_algorithm.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/settings.h"
#include "util/timeline.h"

namespace laser::rule {

class Rule {
  private:
    bool is_existential_m = false;
    std::unique_ptr<formula::Formula> body;
    std::vector<std::unique_ptr<formula::Formula>> head_atoms;
    std::vector<MathAtom> math_atoms;
    // for each atom in head_atoms, there is a vector containing the postions
    // in the body grounding where the value of each head variables is found
    std::vector<std::vector<size_t>> head_atoms_var_positions;
    std::unique_ptr<ChaseFilter> chase_filter;
    // maps head variables names to postion of variables in the body grounding
    std::unordered_map<std::string, int> head_variable_index;
    size_t previous_step = 0;
    size_t current_step = 0;
    std::vector<std::string> frontier_variables;
    std::vector<std::string> bound_variables;
    std::set<std::string> inertia_variables;
    bool has_inertia_variables = false;
    std::vector<std::string> trigger_variables;

    void clear();
    void init_frontier_variables(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);

    std::vector<bool> generate_inertia_vector();

    std::shared_ptr<util::Grounding>
    convert_to_head_grounding(std::string const &head_predicate,
                              util::Grounding const &grounding) const;

    void init_chase(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);

    void init(std::vector<std::unique_ptr<formula::Formula>> head_atoms);

    void expire_head_groundings(util::Timeline const &timeline);

    void
    evaluate_head(util::Timeline const &timeline,
                  util::Database &database,
                  std::vector<std::shared_ptr<util::Grounding>> &body_facts);

    void evaluate_head_atoms(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &body_facts);

    std::shared_ptr<util::Grounding>
    make_atom_fact(std::shared_ptr<util::Grounding> const &body_fact,
                   size_t head_atom_index, size_t atom_arity);

    void evaluate_math_atoms(
        std::vector<std::shared_ptr<util::Grounding>> &body_facts);

  public:
    explicit Rule(std::unique_ptr<formula::Formula> body_formula,
                  std::vector<std::unique_ptr<formula::Formula>> head_atoms);

    explicit Rule(std::unique_ptr<formula::Formula> body_formula,
                  std::vector<std::unique_ptr<formula::Formula>> head_atoms,
                  std::set<std::string> inertia_variables);

    std::unique_ptr<Rule> clone() const;

    // explicit Rule(std::unique_ptr<formula::Formula> body_formula,
    // std::vector<std::unique_ptr<formula::Formula>> head_atoms,
    // std::vector<MathAtom> math_atoms,
    // std::set<std::string> inertia_variables);

    // explicit Rule(std::unique_ptr<formula::Formula> body_formula,
    // std::vector<std::unique_ptr<formula::Formula>> head_atoms);

    ~Rule() = default;

    std::vector<std::string> const &get_frontier_variables() const;
    std::vector<std::string> const &get_bound_variables() const;
    std::vector<std::unique_ptr<formula::Formula>> const &
    get_head_atoms() const;
    std::unique_ptr<formula::Formula> const &get_body() const;

    void
    add_head_atoms(std::vector<std::unique_ptr<formula::Formula>> atom_vector);

    std::set<std::string> get_body_predicates() const;

    std::set<std::string> get_head_predicates() const;

    std::map<std::string, size_t> get_arity_map() const;

    bool is_existential() const;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline);

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline);

    /**
     * Derives new conclusions based on the head of the rule and all
     * groundings that satisfy the body. Also clears previous derivations
     * that no longer hold.
     * @return True if new conclusions have been derived at the current
     * time point and tuple counter
     */
    void evaluate(util::Timeline const &timeline,
                  util::Database const &database);

    void expire_outdated_groundings(util::Timeline const &timeline);

    bool derive_conclusions(util::Timeline const &timeline,
                            util::Database &database);

    void reset_previous_step();

    void set_previous_step(size_t step);
    void set_current_step(uint64_t time, size_t step);
};

} // namespace laser::rule

#endif // LASER_RULE_RULE_H
