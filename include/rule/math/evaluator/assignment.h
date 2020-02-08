#ifndef LASER_FORMULA_MATH_ASSIGNMENT_H
#define LASER_FORMULA_MATH_ASSIGNMENT_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "frmula/math/evaluator.h"
#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "util/grounding.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::formula {

class Assignment : public Evaluator {
  private:
    FormulaType type = FormulaType::ASSIGNMENT;
    std::string variable;
    std::string value;
    std::vector<std::string> predicate_vector;
    std::map<std::string, size_t> arity_map;
    GroundingTable grounding_table;

    void init();

  public:
    explicit Assignment(std::string variable, std::string value);

    ~Assignment() override = default;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    MathOperator get_operator() const override;

    init(std::unique_ptr<formula::Formula> const &body) override;

};

} // namespace laser::formula

#endif // LASER_FORMULA_MATH_ASSIGNMENT_H
