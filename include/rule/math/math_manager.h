#ifndef LASER_RULE_MATH_MATH_MANAGER_H
#define LASER_RULE_MATH_MATH_MANAGER_H

#include <memory>
#include <vector>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/math_operator.h"
#include "rule/math/evaluator.h"
#include "rule/math/evaluator/algebra.h"
#include "rule/math/evaluator/assignment.h"
#include "rule/math/evaluator_type.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser::rule {

class MathManager {
  private:
    std::vector<std::unique_ptr<Evaluator>> evaluators;

    std::vector<std::unique_ptr<Evaluator>>
    compute_evaluators(std::unique_ptr<formula::Formula> const &formula,
                       std::unique_ptr<formula::Formula> const &body) const;

  public:
    void init(std::unique_ptr<formula::Formula> const &body);

    ~MathManager() = default;

    std::vector<std::shared_ptr<util::Grounding>>
    evaluate(util::Timeline const &timeline, 
             std::vector<std::shared_ptr<util::Grounding>> const &facts);

    void expire_outdated_groundings();

    bool has_math_atoms() const;
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_MATH_MANAGER_H
