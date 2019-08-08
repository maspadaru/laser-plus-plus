#ifndef LASER_ACYCLICITY_NAIVE_SMFA_H
#define LASER_ACYCLICITY_NAIVE_SMFA_H

#include <memory>
#include <set>
#include <vector>
#include <algorithm> // std::min()
#include <cmath> // std::ceil()

#include "acyclicity/shared.h"
#include "core/program.h"
#include "formula/formula.h"
#include "rule/rule.h"
#include "util/timeline.h"

namespace laser::acyclicity {

class NaiveSMFA {
  private:
    std::vector<rule::Rule> rule_vector;
    std::vector<rule::Rule> smfa_rule_vector;
    util::Timeline critical_timeline;
    bool has_cycle = false;
    std::set<std::string> extensional_predicates;
    std::map<std::string, size_t> arity_map;
    uint64_t global_tuple_counter = 0;

    uint64_t get_time(formula::Formula *formula, bool is_head_atom);
    std::vector<std::shared_ptr<util::Grounding>> generate_critical_facts();
    void compute_extensional_predicates();
    void compute_critical_timeline();
    void compute_smfa_rule_vector();
    void evaluate_smfa_program();

  public:
    explicit NaiveSMFA(std::vector<rule::Rule> rule_vector);
    bool has_terminating_chase();
};

} // namespace laser::acyclicity

#endif // LASER_ACYCLICITY_NAIVE_SMFA_H
