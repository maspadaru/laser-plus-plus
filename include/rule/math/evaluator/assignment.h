#ifndef LASER_RULE_MATH_EVALUATOR_ASSIGNMENT_H
#define LASER_RULE_MATH_EVALUATOR_ASSIGNMENT_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "formula/math_operator.h"
#include "rule/math/evaluator.h"
#include "rule/math/evaluator_type.h"
#include "util/grounding.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::rule {

class Assignment : public Evaluator {
  private:
    formula::MathOperator math_operator = formula::MathOperator::EQUALS;
    EvaluatorType evaluator_type = EvaluatorType::ASSIGNMENT;
    std::string predicate;
    std::string variable;
    std::string value;
    std::vector<std::string> arguments;
    formula::GroundingTable grounding_table;

  public:
    explicit Assignment(std::string predicate,
                        std::vector<std::string> const &arguments,
                        std::unique_ptr<formula::Formula> const &body);

    ~Assignment() override = default;

    formula::MathOperator get_operator() const override;

    EvaluatorType get_evaluator_type() const override;

    void evaluate(
        util::Timeline const &timeline, 
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings() override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings() override;
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_EVALUATOR_ASSIGNMENT_H
