#include "rule/math/evaluator/assignment.h"

namespace laser::rule {

Assignment::Assignment(std::string predicate,
                       std::vector<std::string> const &arguments,
                       std::unique_ptr<formula::Formula> const &body)
    : predicate(std::move(predicate)) {
    this->arguments = arguments;
    variable = arguments.at(0);
    value = arguments.at(1);
    std::vector<std::string> variable_names;
    variable_names.push_back(this->variable);
    grounding_table.set_variable_names(variable_names);
}

void Assignment::evaluate(
    util::Timeline const &timeline, 
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    std::vector<std::string> value_vector;
    value_vector.push_back(this->value);
    auto ct = timeline.get_time();
    auto ht = timeline.get_time();
    auto cc = timeline.get_tuple_count_at(ct);
    auto hc = timeline.INFINITE_TIME;
    auto grounding = std::make_shared<util::Grounding>(predicate, ct, ht, cc,
                                                       hc, value_vector);
    grounding_table.add_grounding(grounding);
}

void Assignment::expire_outdated_groundings() {
    // clear all, since groundings will be in their respective Math Atoms.
    grounding_table.clear();
}

std::vector<std::shared_ptr<util::Grounding>>
Assignment::get_groundings() {
    return grounding_table.get_recent_groundings();
}

formula::MathOperator Assignment::get_operator() const { return math_operator; }

EvaluatorType Assignment::get_evaluator_type() const { return evaluator_type; }

} // namespace laser::rule
