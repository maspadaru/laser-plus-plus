#ifndef LASER_RULE_MATH_EVALUATOR_H
#define LASER_RULE_MATH_EVALUATOR_H

#include <memory>
#include <vector>

#include "formula/formula.h"
#include "formula/math_operator.h"
#include "rule/math/evaluator_type.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser::rule {

/**
 * Math Evaluator Interface
 */
class Evaluator {
  private:
  public:
    virtual ~Evaluator() = default;

    virtual formula::MathOperator get_operator() const = 0;

    virtual EvaluatorType get_evaluator_type() const = 0;

    /**
     * Returns the recent groundings
     */
    virtual void 
    evaluate(util::Timeline const &timeline,
             std::vector<std::shared_ptr<util::Grounding>> const &facts) = 0;

    virtual void expire_outdated_groundings() = 0;

    /**
     * cleans the grounding table removing all groundings
     */
    virtual std::vector<std::shared_ptr<util::Grounding>>
    get_groundings() = 0;
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_EVALUATOR_H
