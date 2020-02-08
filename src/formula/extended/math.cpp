#include "formula/extended/math.h"

namespace laser::formula {

Math::Math(std::string math_sign, std::vector<std::string> arguments)
    : math_sign(std::move(math_sign)), arguments(std::move(arguments)) {
    init();
}

MathOperator Math::generate_operator(std::string const &math_sign) const {
    auto result = MathOperator::NONE;
    if (math_sign == "=") {
        result = MathOperator::ASSIGNMENT;
    } else if (math_sign == "+") {
        result = MathOperator::PLUS;
    } else if (math_sign == "-") {
        result = MathOperator::MINUS;
    } else if (math_sign == "*") {
        result = MathOperator::MULTIPLICATION;
    } else if (math_sign == "/") {
        result = MathOperator::DIVISION;
    } else if (math_sign == "<") {
        result = MathOperator::LESSER;
    } else if (math_sign == ">") {
        result = MathOperator::GREATHER;
    } else if (math_sign == "<=") {
        result = MathOperator::LESSER_OR_EQUAL;
    } else if (math_sign == ">=") {
        result = MathOperator::GREATHER_OR_EQUAL;
    } else if (math_sign == "?=") {
        result = MathOperator::EQUALS;
    } else if (math_sign == "!=") {
        result = MathOperator::NOT_EQUAL;
    }
    return result;
}

std::string
Math::generate_predicate(std::string const &math_sign,
                         std::vector<std::string> const &arguments) const {
    auto laser_math_pred_str = laser::util::special_predicate::MATH;
    auto pred_math_sign = laser_math_pred_str + math_sign;
    auto result =
        std::accumulate(arguments.begin(), arguments.end(), pred_math_sign);
    return result;
}

void Math::init() {
    math_operator = generate_operator(math_sign);
    predicate = generate_predicate(math_sign, arguments);
    std::vector<std::string> assignment_args;
    if (math_operator == MathOperator::ASSIGNMENT) {
        // assign atom of form: =(VAR, CONST) has only one variable.
        assignment_args.push_back(arguments.at(0));
    } else {
        assignment_args = arguments;
    }
    child =
        std::make_unique<formula::Atom>(predicate, std::move(assignment_args));
}

std::unique_ptr<formula::Formula> Math::clone() const {
    return std::make_unique<formula::Math>(math_sign, arguments);
}

std::vector<std::string> Math::get_math_arguments() const { return arguments; }

MathOperator Math::get_math_operator() const { return math_operator; }

bool Math::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    return child->evaluate(timeline, previous_step, facts);
}

void Math::expire_outdated_groundings(util::Timeline const &timeline) {
    return child->expire_outdated_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Math::get_groundings(util::Timeline const &timeline) {
    return child->get_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Math::get_conclusions_step(util::Timeline const &timeline) {
    return child->get_conclusions_step(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Math::get_conclusions_timepoint(util::Timeline const &timeline) {
    return child->get_conclusions_timepoint(timeline);
}

void Math::set_head(bool is_head) {}

bool Math::is_head() const { return false; }

uint64_t Math::get_window_size() const { return 0; }

FormulaType Math::get_type() const { return formula_type; }

std::vector<std::string> const &Math::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::map<std::string, size_t> const &Math::get_arity_map() const {
    return child->get_arity_map();
}

void Math::add_child(std::unique_ptr<formula::Formula> child) {}

std::vector<std::unique_ptr<formula::Formula> const *>
Math::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    return result;
}

std::vector<std::string> const &Math::get_variable_names() const {
    return child->get_variable_names();
}

int Math::get_variable_index(std::string const &variable_name) const {
    return child->get_variable_index(variable_name);
}

size_t Math::get_number_of_variables() const {
    return child->get_number_of_variables();
}

} // namespace laser::formula
