#ifndef LASER_RULE_RULE_H
#define LASER_RULE_RULE_H

#include <iostream>
#include <memory>
#include <utility>
#include <set>

#include "rule/share.h"
#include "rule/chase_filter.h"
#include "rule/filter/oblivious_filter.h"
#include "rule/filter/skolem_filter.h"
#include "formula/formula.h"
#include "formula/formula_type.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser {
namespace rule {

class Rule {
  private:
    bool is_existential_m = false;
    formula::Formula &body;
    std::vector<formula::Formula *> head_atoms;
    ChaseFilter* chase_filter;
    std::unordered_map<std::string, int> head_variable_index;
    size_t previous_step = 0;

    std::shared_ptr<util::Grounding>
    convert_to_head_grounding(std::string const &head_predicate,
                              util::Grounding const &grounding) const;
        
    void init_chase(std::vector<formula::Formula *> const &head_atoms); 

    void init(std::vector<formula::Formula *> head_atoms);

    void expire_head_groundings(util::Timeline const &timeline);

    void evaluate_head(
        util::Timeline const &timeline, util::Database const &database,
        std::vector<std::shared_ptr<util::Grounding>> const &body_facts);

    void evaluate_head_atoms(
        util::Timeline const &timeline, util::Database const &database,
        std::vector<std::shared_ptr<util::Grounding>> const &body_facts);

    std::shared_ptr<util::Grounding>
    make_atom_fact(std::shared_ptr<util::Grounding> const &body_fact,
                   formula::Formula *atom) const;

  public:
    explicit Rule(formula::Formula *body_formula,
                  std::vector<formula::Formula *> head_atoms);

    Rule(Rule const &other);     // copy constructor
    Rule(Rule &&other) noexcept; // move constructor

    ~Rule();

    Rule &operator=(Rule const &other);     // copy assignment
    Rule &operator=(Rule &&other) noexcept; // move assignment

    formula::Formula &get_head() const;

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
                            util::Database const &database);

    void reset_previous_step();

    void set_previous_step(size_t step);
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_RULE_H
