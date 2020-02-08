#include "rule/math/math_manager.h"

namespace laser::rule {

std::unique_ptr<Evaluator>
generate_evaluator(std::unique_ptr<formula::Formula> const &math_atom,
                   std::unique_ptr<formula::Formula> const &body) {
    std::unique_ptr<Evaluator> result;
    auto predicate = math_atom->get_predicate_vector().at(0);
    auto arguments = math_atom->get_math_arguments();
    auto math_operator = math_atom->get_math_operator();
    switch (math_operator) {
    case formula::MathOperator::ASSIGNMENT:
        result = std::make_unique<Assignment>(predicate, arguments, body);
        break;
    case formula::MathOperator::PLUS:
        break;
    case formula::MathOperator::MINUS:
        break;
    case formula::MathOperator::MULTIPLICATION:
        break;
    case formula::MathOperator::DIVISION:
        break;
    case formula::MathOperator::LESSER:
        break;
    case formula::MathOperator::GREATHER:
        break;
    case formula::MathOperator::LESSER_OR_EQUAL:
        break;
    case formula::MathOperator::GREATHER_OR_EQUAL:
        break;
    case formula::MathOperator::NOT_EQUAL:
        break;
    case formula::MathOperator::EQUALS:
        break;
    case formula::MathOperator::NONE:
        break;
    }
    return result;
}

std::vector<std::unique_ptr<Evaluator>> MathManager::compute_evaluators(
    std::unique_ptr<formula::Formula> const &formula,
    std::unique_ptr<formula::Formula> const &body) const {
    if (formula->get_type() == formula::FormulaType::ATOM) {
        std::vector<std::unique_ptr<Evaluator>> result;
        return result;
    }
    if (formula->get_type() == formula::FormulaType::MATH) {
        auto evaluator = generate_evaluator(formula, body);
        std::vector<std::unique_ptr<Evaluator>> result;
        result.push_back(std::move(evaluator));
        return result;
    }
    if (formula->get_type() == formula::FormulaType::CONJUNCTION) {
        auto children = formula->get_children();
        auto left = compute_evaluators(*children.at(0), body);
        auto right = compute_evaluators(*children.at(1), body);
        left.insert(left.end(), std::make_move_iterator(right.begin()),
                    std::make_move_iterator(right.end()));
        return left;
    }
    auto children = formula->get_children();
    return compute_evaluators(*children.at(0), body);
}

void MathManager::init(std::unique_ptr<formula::Formula> const &body) {
    evaluators = compute_evaluators(body, body);
}

std::vector<std::shared_ptr<util::Grounding>> MathManager::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    std::vector<std::shared_ptr<util::Grounding>> input_facts = facts;
    std::vector<std::shared_ptr<util::Grounding>> result = facts;
    std::vector<std::shared_ptr<util::Grounding>> conclusions;
    bool is_changed = true;
    while (is_changed) {
        is_changed = false;
        for (auto &evaluator : evaluators) {
            evaluator->evaluate(timeline, previous_step, input_facts);
            auto ev_conclusions = evaluator->get_groundings(timeline);
            conclusions.insert(conclusions.end(), ev_conclusions.begin(),
                               ev_conclusions.end());
        }
        if (!conclusions.empty()) {
            result.insert(result.end(), conclusions.begin(), conclusions.end());
            input_facts = std::move(conclusions);
            is_changed = true;
        }
    }
    return result;
}

void MathManager::expire_outdated_groundings(util::Timeline const &timeline) {
    for (auto &evaluator : evaluators) {
        evaluator->expire_outdated_groundings(timeline);
    }
}

bool MathManager::has_math_atoms() const { return !evaluators.empty(); }

} // namespace laser::rule
