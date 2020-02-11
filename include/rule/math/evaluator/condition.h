#ifndef LASER_RULE_MATH_EVALUATOR_CONDITION_H
#define LASER_RULE_MATH_EVALUATOR_CONDITION_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "formula/math_operator.h"
#include "rule/math/evaluator.h"
#include "rule/math/evaluator/value_node.h"
#include "rule/math/evaluator_type.h"
#include "util/grounding.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::rule {

class Condition : public Evaluator {
  private:
    const int TIME_VARIABLE_INDEX = -2;
    const int LEFT_TERM = 0;
    const int RIGHT_TERM = 1;
    EvaluatorType evaluator_type = EvaluatorType::CONDITION;
    formula::MathOperator math_operator;
    uint64_t max_time_window = 0;
    uint64_t max_tuple_window = 0;
    std::string predicate;
    std::vector<std::string> variable_names;
    formula::GroundingTable grounding_table;
    value_set left_value_set;
    value_set right_value_set;
    /** key - predicate;
     * value - vector containing position of left (or right) vars in atom with
     * this predicate.
     * */
    std::unordered_map<std::string, std::unordered_set<int>> left_var_map;
    std::unordered_map<std::string, std::unordered_set<int>> right_var_map;

    bool is_integer(std::string const &inputString, int64_t &result) const;
    void update_window_size(std::unique_ptr<formula::Formula> const &formula);

    void update_var_map_time_reference(
        std::unique_ptr<formula::Formula> const &formula);
    void update_var_map_atom(std::unique_ptr<formula::Formula> const &formula);
    void init_var_map(std::unique_ptr<formula::Formula> const &formula);

    uint64_t compute_annotation(uint64_t min_window, uint64_t left,
                                uint64_t right) const;

    std::vector<std::shared_ptr<util::Grounding>>
    generate_groundings(util::Timeline const &timeline) const;

    bool check_condition(value_node const &left_node,
                         value_node const &right_node) const;
    bool check_numeric_condition(value_node const &left_node,
                                 value_node const &right_node) const;
    bool check_string_condition(value_node const &left_node,
                                value_node const &right_node) const;

    void update_value_set(
        util::Timeline const &timeline,
        std::shared_ptr<util::Grounding> const &fact,
        std::unordered_map<std::string, std::unordered_set<int>> const &var_map,
        value_set &values) const;

  public:
    explicit Condition(formula::MathOperator math_operator,
                       std::string predicate,
                       std::vector<std::string> arguments,
                       std::unique_ptr<formula::Formula> const &body);

    ~Condition() override = default;

    formula::MathOperator get_operator() const override;

    EvaluatorType get_evaluator_type() const override;

    void evaluate(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings() override;

    std::vector<std::shared_ptr<util::Grounding>> get_groundings() override;
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_EVALUATOR_CONDITION_H
