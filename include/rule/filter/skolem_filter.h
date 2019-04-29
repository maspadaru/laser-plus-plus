#ifndef LASER_RULE_FILTER_SKOLEM_FILTER_H
#define LASER_RULE_FILTER_SKOLEM_FILTER_H

#include <memory>
#include <vector>

#include "formula/formula.h"
#include "rule/chase_filter.h"
#include "rule/shared.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser {
namespace rule {

/**
 * Skolem Chase Filter
 */
class SkolemFilter : public ChaseFilter {
  private:
    std::vector<std::string> head_variables;
    std::vector<std::string> free_variables;
    std::vector<std::string> bound_variables;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::unordered_map<size_t, std::vector<std::string>> skolem_map;
    uint64_t null_value_count = 0;

    std::shared_ptr<util::Grounding>
    generate_chase_fact(std::shared_ptr<util::Grounding> const &input_fact);

    std::string generate_new_value(std::string const &var_name);

  public:
    SkolemFilter() = default;
    ~SkolemFilter() override = default;

    ChaseFilter *create() const override;
    ChaseFilter *clone() const override;
    ChaseFilter *move() override;

    void init(std::vector<formula::Formula *> const &head_atoms,
              std::vector<std::string> const &head_variables,
              std::vector<std::string> const &free_variables,
              std::vector<std::string> const &bound_variables) override;

    void update(util::Timeline const &timeline,
                util::Database const &database) override;

    std::vector<std::shared_ptr<util::Grounding>>
    build_chase_facts(util::Timeline const &timeline,
                      std::vector<std::shared_ptr<util::Grounding>> const
                          &input_facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_FILTER_SKOLEM_FILTER_H
