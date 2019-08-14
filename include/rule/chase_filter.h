#ifndef LASER_RULE_CHASE_FILTER_H
#define LASER_RULE_CHASE_FILTER_H

#include <memory>
#include <vector>

#include "formula/formula.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

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

    virtual void init(std::vector<formula::Formula *> const &head_atoms,
                      std::vector<std::string> const &head_variables,
                      std::vector<std::string> const &free_variables,
                      std::vector<std::string> const &bound_variables,
                      std::vector<bool> const &is_inertia_variable,
                      std::vector<std::string> const &frontier_variables, 
                      bool has_inertia_variables) = 0;

    virtual void update(util::Timeline const &timeline, size_t previous_step,
                        util::Database const &database) = 0;

    virtual std::vector<std::shared_ptr<util::Grounding>> build_chase_facts(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &input_facts) = 0;

    virtual void expire_outdated_groundings(util::Timeline const &timeline) = 0;
};

} // namespace rule
} // namespace laser

#endif // LASER_RULE_CHASE_FILTER_H
