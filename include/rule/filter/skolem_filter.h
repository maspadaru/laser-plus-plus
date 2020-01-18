#ifndef LASER_RULE_FILTER_SKOLEM_FILTER_H
#define LASER_RULE_FILTER_SKOLEM_FILTER_H

#include <iterator> // std::ostream_iterator
#include <map>
#include <memory>
#include <set>
#include <sstream> // std::stringstream
#include <vector>

#include "formula/formula.h"
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
 * Skolem Chase Filter
 */
class SkolemFilter : public ChaseFilter {
  private:
    uint64_t current_timepoint = 0;
    bool has_inertia_variables;
    std::vector<std::string> head_variables;
    std::vector<std::string> free_variables;
    std::vector<std::string> frontier_variables;
    std::vector<std::string> bound_variables;
    std::vector<bool> is_inertia_variable;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::map<std::string, std::vector<std::string>> skolem_map;
    std::map<std::string, std::vector<std::string>> inertia_map;
    uint64_t null_value_count = 0;
    bool use_global_nulls = true;

    /** Returns a new grounding that contains only the values of the fronteer
     * variables */
    std::string
    compute_key(std::shared_ptr<util::Grounding> const &input_fact) const;

    std::shared_ptr<util::Grounding>
    generate_chase_fact(std::shared_ptr<util::Grounding> const &input_fact);

    std::string generate_new_value(std::string const &var_name);

    std::string concatenate(std::vector<std::string> const &source,
                            std::string const &delimiter) const;

    std::vector<std::string> const &
    generate_new_values(std::string const &skolem_key);

    std::vector<std::string> const &
    get_bound_values(std::string const &skolem_key);

    std::vector<std::string> const &
    get_bound_values_with_inertia(std::string const &skolem_key);

  public:
    SkolemFilter() = default;
    ~SkolemFilter() override = default;

    //ChaseFilter *create() const override;
    //ChaseFilter *clone() const override;
    //ChaseFilter *move() override;

    void init(std::vector<std::unique_ptr<formula::Formula>> const &head_atoms,
              std::vector<std::string> const &head_variables,
              std::vector<std::string> const &free_variables,
              std::vector<std::string> const &bound_variables,
              std::vector<bool> const &is_inertia_variable,
              std::vector<std::string> const &frontier_variables,
              bool has_inertia_variables) override;

    void update(util::Timeline const &timeline, size_t previous_step,
                util::Database &database) override;

    std::vector<std::shared_ptr<util::Grounding>>
    build_chase_facts(util::Timeline const &timeline, size_t previous_step,
                      std::vector<std::shared_ptr<util::Grounding>> const
                          &input_facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;
};

} // namespace laser::rule

#endif // LASER_RULE_FILTER_SKOLEM_FILTER_H
