#include "rule/math/evaluator/algebra.h"

namespace laser::rule {
// private:

void Algebra::update_var_map(std::unique_ptr<formula::Formula> const &formula) {
    auto predicate = formula->get_predicate_vector().at(0);
    auto left_index = formula->get_variable_index(variable_names.at(LEFT_TERM));
    if (left_index >= 0) {
        auto &left_set = left_var_map[predicate];
        left_set.insert(left_index);
    }
    auto right_index =
        formula->get_variable_index(variable_names.at(RIGHT_TERM));
    if (right_index >= 0) {
        auto &right_set = right_var_map[predicate];
        right_set.insert(right_index);
    }
}

void Algebra::update_window_size(
    std::unique_ptr<formula::Formula> const &formula) {
    if (formula->get_type() == formula::FormulaType::TIME_WINDOW) {
        auto win_size = formula->get_window_size();
        if (max_time_window < win_size) {
            max_time_window = win_size;
        }
    } else if (formula->get_type() == formula::FormulaType::TUPLE_WINDOW) {
        auto win_size = formula->get_window_size();
        if (max_tuple_window < win_size) {
            max_tuple_window = win_size;
        }
    }
}

void Algebra::init_var_map(std::unique_ptr<formula::Formula> const &formula) {
    if (formula->get_type() == formula::FormulaType::ATOM) {
        update_var_map(formula);
    } else {
        auto children = formula->get_children();
        init_var_map(*children.at(0));
        if (formula->get_type() == formula::FormulaType::CONJUNCTION) {
            init_var_map(*children.at(1));
        }
    }
    update_window_size(formula);
}

double Algebra::do_math(value_node const &left_node,
                        value_node const &right_node) const {
    double left_value = left_node.num_value;
    double right_value = right_node.num_value;
    double result = 0;
    switch (math_operator) {
    case formula::MathOperator::PLUS:
        result = left_value + right_value;
        break;
    case formula::MathOperator::MINUS:
        result = left_value - right_value;
        break;
    case formula::MathOperator::MULTIPLICATION:
        result = left_value * right_value;
        break;
    case formula::MathOperator::DIVISION:
        result = left_value / right_value;
        break;
    default:
        result = 0;
    }
    return result;
}

bool Algebra::is_double(std::string const &inputString, double &result) const {
    char *end;
    result = std::strtod(inputString.c_str(), &end);
    return !(end == inputString.c_str() || *end != '\0');
}

std::vector<std::shared_ptr<util::Grounding>>
Algebra::generate_groundings(util::Timeline const &timeline,
                             value_set const &left_set,
                             value_set const &right_set) const {
    auto ct = timeline.get_time();
    auto cc = timeline.get_tuple_count_at(ct);
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &left_node : left_set) {
        for (auto const &right_node : right_set) {
            auto result_value = do_math(left_node, right_node);
            std::vector<std::string> value_vector;
            value_vector.push_back(std::to_string(result_value));
            value_vector.push_back(left_node.str_value);
            value_vector.push_back(right_node.str_value);
            auto ht =
                (left_node.ht > right_node.ht) ? right_node.ht : left_node.ht;
            auto hc =
                (left_node.hc > right_node.hc) ? right_node.hc : left_node.hc;
            auto grounding = std::make_shared<util::Grounding>(
                predicate, ct, ht, cc, hc, value_vector);
            result.push_back(grounding);
        }
    }
    return result;
}

void Algebra::update_value_set(
    std::shared_ptr<util::Grounding> const &fact,
    std::unordered_map<std::string, std::unordered_set<size_t>> const &var_map,
    value_set &values) const {
    auto const &predicate = fact->get_predicate();
    if (var_map.count(predicate) > 0) {
        auto indices = var_map.at(predicate);
        for (auto index : indices) {
            auto str_value = fact->get_constant(index);
            double num_value;
            if (is_double(str_value, num_value)) {
                value_node node;
                node.num_value = num_value;
                node.str_value = str_value;
                node.ht = fact->get_horizon_time() + max_time_window;
                node.hc = fact->get_horizon_count() + max_tuple_window;
                values.insert(node);
            }
        }
    }
}

// public :

Algebra::Algebra(formula::MathOperator math_operator, std::string predicate,
                 std::vector<std::string> arguments,
                 std::unique_ptr<formula::Formula> const &body)
    : math_operator(math_operator), predicate(std::move(predicate)),
      variable_names(std::move(arguments)) {
    grounding_table.set_variable_names(variable_names);
    init_var_map(body);
}

void Algebra::evaluate(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    value_set left_set;
    value_set right_set;
    for (auto const &fact : facts) {
        // I need to assemble two lists of all values I find for left & right.
        update_value_set(fact, left_var_map, left_set);
        update_value_set(fact, right_var_map, right_set);
    }
    auto new_groundings = generate_groundings(timeline, left_set, right_set);
    grounding_table.add_grounding_vector(new_groundings);
}

void Algebra::expire_outdated_groundings() { grounding_table.clear(); }

std::vector<std::shared_ptr<util::Grounding>> Algebra::get_groundings() {
    return grounding_table.get_recent_groundings();
}

formula::MathOperator Algebra::get_operator() const { return math_operator; }

EvaluatorType Algebra::get_evaluator_type() const { return evaluator_type; }

} // namespace laser::rule
