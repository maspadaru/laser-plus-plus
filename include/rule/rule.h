//
// Created by mike on 6/15/18.
//
#ifndef LASER_RULE_RULE_H
#define LASER_RULE_RULE_H

#include <iostream>
#include <memory>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "util/grounding.h"
#include "util/timeline.h"
#include "util/database.h"

namespace laser {
namespace rule {

class Rule {
  private:
    formula::Formula &head;
    formula::Formula &body;

    size_t previous_step = 0;

    /**
     * Maps variables positions in the head's Grounding Table to positions in
     * the body's Grounding Table.
     * E.g.: p(X, Y) :- q(Y, Z, X) -> { [0, 2], [1, 0] }
     */
    std::unordered_map<size_t, size_t> variable_map;

    std::shared_ptr<util::Grounding>
    convert_to_head_grounding(std::string const &head_predicate,
                              util::Grounding const &grounding) const;

    void compute_variable_map();

    void init();

  public:
    explicit Rule(formula::Formula *head_formula,
                  formula::Formula *body_formula);

    Rule(Rule const &other);     // copy constructor
    Rule(Rule &&other) noexcept; // move constructor

    ~Rule();

    Rule &operator=(Rule const &other);     // copy assignment
    Rule &operator=(Rule &&other) noexcept; // move assignment

    formula::Formula &get_head() const;

    bool is_existential() const;

    /**
     * Derives new conclusions based on the head of the rule and all groundings
     * that satisfy the body.
     * Also clears previous derivations that no longer hold.
     * @return True if new conclusions have been derived at the current
     * time point and tuple counter
     */
    void evaluate(util::Timeline const &timeline,
                  util::Database const &database);

    void expire_outdated_groundings(util::Timeline const &timeline);

    bool derive_conclusions(util::Timeline const &timeline);

    void reset_previous_step();

    void set_previous_step(size_t step);
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_RULE_H
