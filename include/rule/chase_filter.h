#ifndef LASER_RULE_CHASE_FILTER_H
#define LASER_RULE_CHASE_FILTER_H

#include <memory>
#include <vector>

#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"
#include "formula/formula.h"

namespace laser {
namespace rule {

/**
 * Chase Filter Interface
 */
class ChaseFilter {
  public:
    virtual ~ChaseFilter() = default;

    virtual ChaseFilter *create() const = 0; // Virtual constructor (creation)
    virtual ChaseFilter *clone() const = 0;  // Virtual constructor (copying)
    virtual ChaseFilter *move() = 0;

    virtual void init(std::vector<formula::Formula *> const &head_atoms) = 0;

    virtual void update(util::Timeline const &timeline,
                        util::Database const &database) = 0;

    virtual std::vector<std::shared_ptr<util::Grounding>> build_chase_facts(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &input_facts) = 0;
    
};

} // namespace rule
} // namespace laser 

#endif // LASER_RULE_CHASE_FILTER_H
