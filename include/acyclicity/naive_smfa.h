#ifndef LASER_ACYCLICITY_NAIVE_SMFA_H
#define LASER_ACYCLICITY_NAIVE_SMFA_H

#include <algorithm> // std::min()
#include <cmath>     // std::ceil()
#include <memory>
#include <set>
#include <vector>

#include "acyclicity/shared.h"
#include "core/program.h"
#include "formula/extended/atom.h"
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
    std::string generate_new_F_predicate(size_t rule_index,
                                         size_t bound_index) const;
    formula::Formula *
    generate_new_unary_atom(std::string const &predicate,
                            std::string const &variable) const;
    formula::Formula *
    generate_new_binary_atom(std::string const &predicate,
                             std::string const &frist_variable,
                             std::string const &second_variable) const;

    rule::Rule generate_S_D_rule() const; 

    rule::Rule generate_D_transitive_rule() const; 
            
    rule::Rule generate_C_rule(std::string const &predicate_F) const;

  public:
    explicit NaiveSMFA(std::vector<rule::Rule> rule_vector);
    bool has_terminating_chase();
};

} // namespace laser::acyclicity

#endif // LASER_ACYCLICITY_NAIVE_SMFA_H
