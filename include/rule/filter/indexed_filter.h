#ifndef LASER_RULE_FILTER_INDEXED_FILTER_H
#define LASER_RULE_FILTER_INDEXED_FILTER_H

#include <map>
#include <unordered_map>
#include <memory>
#include <set>
#include <vector>

#include "formula/extended/conjunction.h"
#include "formula/formula.h"
#include "formula/formula_type.h"
#include "rule/chase_filter.h"
#include "rule/shared.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser {
namespace rule {

/**
 * Restricted Chase Filter using free variable based index
 */
class IndexedFilter : public ChaseFilter {
  private:
    uint64_t null_value_count = 0;
    formula::Formula *head_formula;
    std::vector<std::string> free_head_variables;
    std::vector<std::string> head_variables;
    std::vector<std::string> free_variables;
    std::vector<std::string> bound_variables;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::unordered_map<std::string, std::shared_ptr<util::Grounding>> index_map;

    std::shared_ptr<util::Grounding>
    generate_chase_fact(std::shared_ptr<util::Grounding> const &input_fact);

    std::string generate_new_value(std::string const &var_name);

    bool has_database_match(
        std::shared_ptr<util::Grounding> const &input_fact) const;

    bool compare_horizon_time(
        std::shared_ptr<util::Grounding> const &db_grounding,
        std::shared_ptr<util::Grounding> const &input_grounding) const;

    formula::Formula *
    build_head_formula(size_t index,
                       std::vector<formula::Formula *> const &list) const;

    void
    init_free_head_variables(std::vector<formula::Formula *> const &head_atoms);

    void clear_index_map();
    void compute_index_map(
        std::vector<std::shared_ptr<util::Grounding>> database_facts);

    std::string
    get_index_key(std::shared_ptr<util::Grounding> const &fact) const;

  public:
    IndexedFilter() = default;
    ~IndexedFilter() override;

    ChaseFilter *create() const override;
    ChaseFilter *clone() const override;
    ChaseFilter *move() override;

    void init(std::vector<formula::Formula *> const &head_atoms,
              std::vector<std::string> const &head_variables,
              std::vector<std::string> const &free_variables,
              std::vector<std::string> const &bound_variables) override;

    void update(util::Timeline const &timeline, size_t previous_step,
                util::Database const &database) override;

    std::vector<std::shared_ptr<util::Grounding>>
    build_chase_facts(util::Timeline const &timeline, size_t previous_step,
                      std::vector<std::shared_ptr<util::Grounding>> const
                          &input_facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_FILTER_INDEXED_FILTER_H
