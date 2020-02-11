#include "rule/math/evaluator/algebra.h"

namespace laser::rule {
// private:

void Algebra::update_var_map_time_reference(
    std::unique_ptr<formula::Formula> const &formula) {
    // The time variable is always the last one
    auto formula_predicate = formula->get_predicate_vector().at(0);
    auto time_ref_variables = formula->get_variable_names();
    auto left_index = formula->get_variable_index(variable_names.at(LEFT_TERM));
    if (left_index == time_ref_variables.size() - 1) {
        auto &left_set = left_var_map[formula_predicate];
        left_set.insert(TIME_VARIABLE_INDEX);
    }
    auto right_index =
        formula->get_variable_index(variable_names.at(RIGHT_TERM));
    if (right_index == time_ref_variables.size() - 1) {
        auto &right_set = right_var_map[formula_predicate];
        right_set.insert(TIME_VARIABLE_INDEX);
    }
}

void Algebra::update_var_map_atom(
    std::unique_ptr<formula::Formula> const &formula) {
    auto formula_predicate = formula->get_predicate_vector().at(0);
    auto left_index = formula->get_variable_index(variable_names.at(LEFT_TERM));
    if (left_index >= 0) {
        auto &left_set = left_var_map[formula_predicate];
        left_set.insert(left_index);
    }
    auto right_index =
        formula->get_variable_index(variable_names.at(RIGHT_TERM));
    if (right_index >= 0) {
        auto &right_set = right_var_map[formula_predicate];
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
        update_var_map_atom(formula);
    } else {
        auto children = formula->get_children();
        init_var_map(*children.at(0));
        if (formula->get_type() == formula::FormulaType::CONJUNCTION) {
            init_var_map(*children.at(1));
        }
        if (formula->get_type() == formula::FormulaType::TIME_REFERENCE) {
            update_var_map_time_reference(formula);
        }
    }
    update_window_size(formula);
}

long Algebra::do_math(value_node const &left_node,
                        value_node const &right_node) const {
    long left_value = left_node.num_value;
    long right_value = right_node.num_value;
    long result = 0;
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

bool Algebra::is_integer(std::string const &inputString, long &result) const {
    char *end;
    result = std::strtol(inputString.c_str(), &end, 10);
    return !(end == inputString.c_str() || *end != '\0');
}

uint64_t Algebra::compute_annotation(uint64_t min_window, uint64_t left,
                                     uint64_t right) const {
    // annotation should be smallest between left and right
    // but never smaller than min_window
    auto min_left_right = (left < right) ? left : right;
    return (min_window > min_left_right) ? min_window : min_left_right;
}

std::vector<std::shared_ptr<util::Grounding>>
Algebra::generate_groundings(util::Timeline const &timeline) const {
    auto ct = timeline.get_time();
    auto cc = timeline.get_tuple_count_at(ct);
    auto min_ht = ct + max_time_window;
    auto min_hc = cc + max_tuple_window;
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &left_node : left_value_set) {
        for (auto const &right_node : right_value_set) {
            auto result_value = do_math(left_node, right_node);
            std::vector<std::string> value_vector;
            value_vector.push_back(std::to_string(result_value));
            value_vector.push_back(left_node.str_value);
            value_vector.push_back(right_node.str_value);
            auto ht = compute_annotation(min_ht, left_node.ht, right_node.ht);
            auto hc = compute_annotation(min_hc, left_node.hc, right_node.hc);
            auto grounding = std::make_shared<util::Grounding>(
                predicate, ct, ht, cc, hc, value_vector);
            result.push_back(grounding);
        }
    }
    return result;
}

void Algebra::update_value_set(
    util::Timeline const &timeline,
    std::shared_ptr<util::Grounding> const &fact,
    std::unordered_map<std::string, std::unordered_set<int>> const &var_map,
    value_set &values) const {
    auto const &predicate = fact->get_predicate();
    if (var_map.count(predicate) > 0) {
        auto indices = var_map.at(predicate);
        for (auto index : indices) {
            bool found = false;
            std::string str_value;
            long num_value;
            if (index == TIME_VARIABLE_INDEX) {
                num_value = timeline.get_time();
                str_value = std::to_string(num_value);
                found = true;
            } else {
                str_value = fact->get_constant(index);
                found = is_integer(str_value, num_value);
            }
            if (found) {
                value_node node;
                node.num_value = num_value;
                node.str_value = str_value;
                node.ht = fact->get_horizon_time();
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
    for (auto const &fact : facts) {
        // I need to assemble two lists of all values I find for left & right.
        update_value_set(timeline, fact, left_var_map, left_value_set);
        update_value_set(timeline, fact, right_var_map, right_value_set);
    }
    auto new_groundings = generate_groundings(timeline);
    grounding_table.add_grounding_vector(new_groundings);
}

void Algebra::expire_outdated_groundings() {
    left_value_set.clear();
    right_value_set.clear();
    grounding_table.clear();
}

std::vector<std::shared_ptr<util::Grounding>> Algebra::get_groundings() {
    return grounding_table.get_recent_groundings();
}

formula::MathOperator Algebra::get_operator() const { return math_operator; }

EvaluatorType Algebra::get_evaluator_type() const { return evaluator_type; }

} // namespace laser::rule
