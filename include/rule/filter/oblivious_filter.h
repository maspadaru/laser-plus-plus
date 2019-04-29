#ifndef LASER_RULE_FILTER_OBLIVIOUS_FILTER_H
#define LASER_RULE_FILTER_OBLIVIOUS_FILTER_H

#include <memory>
#include <vector>

#include "rule/chase_filter.h"
#include "rule/share.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"
#include "formula/formula.h"

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

    void init(std::vector<formula::Formula *> const &head_atoms) override;

    void update(util::Timeline const &timeline,
                        util::Database const &database) override;

    std::vector<std::shared_ptr<util::Grounding>> build_chase_facts(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &input_facts) override;
    
};

} // namespace rule
} // namespace laser 

#endif // LASER_RULE_FILTER_OBLIVIOUS_FILTER_H
