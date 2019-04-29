#ifndef LASER_RULE_FILTER_OBLIVIOUS_FILTER_H
#define LASER_RULE_FILTER_OBLIVIOUS_FILTER_H

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
 * Oblivious Chase Filter
 */
class ObliviousFilter : public ChaseFilter {
  public:
    ObliviousFilter() = default;
    ~ObliviousFilter() override = default;

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

#endif // LASER_RULE_FILTER_OBLIVIOUS_FILTER_H
