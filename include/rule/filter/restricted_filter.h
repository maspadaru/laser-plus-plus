#ifndef LASER_RULE_FILTER_RESTRICTED_FILTER_H
#define LASER_RULE_FILTER_RESTRICTED_FILTER_H

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "formula/extended/conjunction.h"
#include "formula/formula.h"
#include "formula/formula_type.h"
#include "rule/chase_filter.h"
#include "rule/shared.h"
#include "util/database.h"
#include "util/global.h"
#include "util/grounding.h"
#include "util/settings.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::rule {

/**
 * Restricted Chase Filter
 */
class RestrictedFilter : public ChaseFilter {
  private:
    uint64_t null_value_count = 0;
    size_t head_variables_count = 0;
    std::vector<std::string> frontier_variables;
    std::vector<std::string> head_variables;
    std::vector<std::string> free_variables;
    std::vector<std::string> bound_variables;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::unordered_map<std::string, int> head_variable_index;
    std::vector<std::shared_ptr<util::Grounding>> inertia_facts;
    std::vector<std::shared_ptr<util::Grounding>> current_timepoint_facts;
    std::vector<std::shared_ptr<util::Grounding>> current_step_facts;
    size_t head_atom_count = 0;
    uint64_t current_timepoint;
    bool has_event_variables = false;
    std::vector<bool> is_event_variable;
    bool use_global_nulls = true;
    bool has_database_facts = false;
    std::string const BOUND_VALUE_PLACEHOLDER = "";

    std::vector<std::vector<std::string const &>> current_step_substitutions;
    std::vector<std::vector<std::string const &>> current_timepoint_substitutions;
    std::vector<std::vector<std::string const &>> inertia_substitutions;



    // position of event variables in head_variables
    std::vector<int> event_variable_list;

    // index = head_atom,
    // subvector index = variable index in atom
    // subvector value = variable index in head_variables; -1 if absent
    std::vector<std::vector<int>> head_atoms_var_index_atom_to_head;


    // index = variable position in head_variable vector
    // value = variable position in input_fact (free_variables vector)
    std::vector<int> body_head_var_index;

    std::vector<std::string> head_atom_predicates;

    void init_body_head_var_index();
    void init_head_atom_predicates(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);

    void init_head_atoms_var_index_atom_to_head(
        std::vector<std::unique_ptr<formula::Formula>> const &head_atoms);

    void init_database_facts(util::Database &database);

    std::vector<std::vector<std::shared_ptr<util::Grounding>> const &>
        database_facts; // positions correspond to head_atoms

    std::vector<std::string const &> extract_substitution(
        std::shared_ptr<util::Grounding> const &input_fact) const;

    std::shared_ptr<util::Grounding>
    convert_to_chase_fact(std::shared_ptr<util::Grounding> const &db_fact);

    std::shared_ptr<util::Grounding> generate_chase_fact_from_inertia(
        std::shared_ptr<util::Grounding> const &inertia_fact);

    std::shared_ptr<util::Grounding>
    generate_chase_fact(std::shared_ptr<util::Grounding> const &input_fact);

    std::string generate_new_value(std::string const &var_name);

    void search_database(std::shared_ptr<util::Grounding> const &input_fact);

    void find_match(std::vector<std::string const &> substitution,
                    size_t head_atom_index);

    void
    find_match(std::vector<std::shared_ptr<util::Grounding>> const &database,
               std::shared_ptr<util::Grounding> const &input_fact);

    bool is_database_match(
        std::shared_ptr<util::Grounding> const &db_grounding,
        std::shared_ptr<util::Grounding> const &input_grounding) const;

    bool is_event_variable_match(
        std::shared_ptr<util::Grounding> const &db_grounding,
        std::shared_ptr<util::Grounding> const &input_grounding) const;

    bool is_frontier_variable_match(
        std::shared_ptr<util::Grounding> const &db_grounding,
        std::shared_ptr<util::Grounding> const &input_grounding) const;

    // std::unique_ptr<formula::Formula> build_head_formula(
    // size_t index,
    // std::vector<std::unique_ptr<formula::Formula>> const &list) const;

  public:
    RestrictedFilter() = default;

    void init(std::vector<std::unique_ptr<formula::Formula>> const &head_atoms,
              std::vector<std::string> const &head_variables,
              std::vector<std::string> const &free_variables,
              std::vector<std::string> const &bound_variables,
              std::vector<bool> const &is_event_variable,
              std::vector<std::string> const &frontier_variables,
              bool has_event_variables) override;

    void update(util::Timeline const &timeline, size_t previous_step,
                util::Database &database) override;

    std::vector<std::shared_ptr<util::Grounding>>
    build_chase_facts(util::Timeline const &timeline, size_t previous_step,
                      std::vector<std::shared_ptr<util::Grounding>> const
                          &input_facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;
};

} // namespace laser::rule

#endif // LASER_RULE_FILTER_RESTRICTED_FILTER_H
