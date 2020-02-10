#ifndef LASER_RULE_MATH_EVALUATOR_ALGEBRA_H
#define LASER_RULE_MATH_EVALUATOR_ALGEBRA_H

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
#include "rule/math/evaluator_type.h"
#include "util/grounding.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::rule {

struct value_node {
    double num_value;
    uint64_t ht;
    uint64_t hc;
    std::string str_value;
};

struct value_node_compare {
    bool operator()(value_node const &x, value_node const &y) const {
        return x.num_value < y.num_value;
    }
};

using value_set = std::set<value_node, value_node_compare>;

class Algebra : public Evaluator {
  private:
    const int TIME_VARIABLE_INDEX = -2;
    const int RESULT_VAR = 0;
    const int LEFT_TERM = 1;
    const int RIGHT_TERM = 2;
    EvaluatorType evaluator_type = EvaluatorType::ALGEBRA;
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

    bool is_double(std::string const &inputString, double &result) const;
    void update_window_size(std::unique_ptr<formula::Formula> const &formula);

    void update_var_map_time_reference(
        std::unique_ptr<formula::Formula> const &formula);
    void update_var_map_atom(std::unique_ptr<formula::Formula> const &formula);
    void init_var_map(std::unique_ptr<formula::Formula> const &formula);

    uint64_t compute_annotation(uint64_t min_window, uint64_t left,
                                uint64_t right) const;

    std::vector<std::shared_ptr<util::Grounding>>
    generate_groundings(util::Timeline const &timeline) const;

    double do_math(value_node const &left_node,
                   value_node const &right_node) const;

    void update_value_set(
        util::Timeline const &timeline,
        std::shared_ptr<util::Grounding> const &fact,
        std::unordered_map<std::string, std::unordered_set<int>> const
            &var_map,
        value_set &values) const;

  public:
    explicit Algebra(formula::MathOperator math_operator, std::string predicate,
                     std::vector<std::string> arguments,
                     std::unique_ptr<formula::Formula> const &body);

    ~Algebra() override = default;

    formula::MathOperator get_operator() const override;

    EvaluatorType get_evaluator_type() const override;

    void evaluate(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings() override;

    std::vector<std::shared_ptr<util::Grounding>> get_groundings() override;
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_EVALUATOR_ALGEBRA_H
