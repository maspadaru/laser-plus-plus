#include "rule/math/evaluator/assignment.h"

namespace laser::rule {

Assignment::Assignment(std::string const &predicate,
                       std::vector<std::string> const &arguments,
                       std::unique_ptr<formula::Formula> const &body)
    : predicate(predicate) {
    this->arguments = arguments;
    variable = arguments.at(0);
    value = arguments.at(1);
    std::vector<std::string> variable_names;
    variable_names.push_back(this->variable);
    grounding_table.set_variable_names(variable_names);
}

bool Assignment::evaluate(
    util::Timeline const &timeline, size_t previous_step,
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
    return true;
}

void Assignment::expire_outdated_groundings(util::Timeline const &timeline) {
    grounding_table.clear();
}

std::vector<std::shared_ptr<util::Grounding>>
Assignment::get_groundings(util::Timeline const &timeline) {
    return grounding_table.get_recent_groundings();
}

formula::MathOperator Assignment::get_operator() const { return math_operator; }

EvaluatorType Assignment::get_evaluator_type() const { return evaluator_type; }

} // namespace laser::rule
