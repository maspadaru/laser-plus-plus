#ifndef LASER_FORMULA_MATH_EVALUATOR_H
#define LASER_FORMULA_MATH_EVALUATOR_H

#include <map>
#include <memory>
#include <string>

#include "math_operator.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser::formula {

/**
 * Math Evaluator Interface
 */
class Formula {
  private:
  public:
    virtual ~Formula() = default;

    virtual MathOperator get_operator() const = 0;

    virtual init(std::unique_ptr<formula::Formula> const &body) = 0;

    /**
     * Returns the recent groundings
     */ 
    virtual bool
    evaluate(util::Timeline const &timeline, size_t previous_step,
             std::vector<std::shared_ptr<util::Grounding>> const &facts) = 0;

    virtual void expire_outdated_groundings(util::Timeline const &timeline) = 0;

    /**
     * cleans the grounding table removing all groundings
     */ 
    virtual std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) = 0;
};

} // namespace laser::formula

#endif // LASER_FORMULA_MATH_EVALUATOR_H
