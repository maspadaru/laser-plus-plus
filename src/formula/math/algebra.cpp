#include "formula/math/algebra.h"

namespace laser::formula {
// private:

void Algebra::init() {
    grounding_table.set_variable_names(variable_names);
    auto predicate = laser::util::special_predicate::MATH_ALGEBRA;
    this->arity_map.try_emplace(predicate, 3);
    this->predicate_vector.push_back(std::move(predicate));
}

void Algebra::compute_var_map(std::unique_ptr<formula::Formula> const &formula) {
    if (formula->get_type() == FormulaType::ATOM) {
        var_map

    }

    if (formula->get_type() == FormulaType::CONJUNCTION) {

    }
}

void Algebra::set_body_reference(std::unique_ptr<formula::Formula> const &body) {
    compute_var_map(body);



}

    // public :

    Algebra::Algebra(MathOperator oprt, std::vector<std::string> arguments)
    : oprt(oprt), variable_names(std::move(arguments)) {
    init();
}

std::unique_ptr<formula::Formula> Algebra::clone() const {
    return std::make_unique<formula::Algebra>(oprt, variable_names);
}

bool Algebra::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    auto max_ht = timeline.get_time();
    for (auto const &fact : facts) {
        // I need to assemble two lists of all values I find for left & right.
        auto predicate = fact->get_predicate();
    }
    for () {
        for () {
            // Do all combinations between values in these lists.
            // Each combo I compute result and assemble a grounding.
            // Just remember the longest ht from all groundings.
            auto predicate = laser::util::special_predicate::MATH_ASSIGNMENT;
            std::vector<std::string> value_vector;
            value_vector.push_back(); // result
            value_vector.push_back(); // left
            value_vector.push_back(); // right
            auto ct = timeline.get_time();
            auto ht = max_ht;
            auto cc = timeline.get_tuple_count_at(ct);
            auto hc = timeline.INFINITE_TIME;
            auto grounding = std::make_shared<util::Grounding>(
                predicate, ct, ht, cc, hc, value_vector);
            grounding_table.add_grounding(grounding);
        }
    }
    return true;
}

// not interesting:

void Algebra::expire_outdated_groundings(util::Timeline const &timeline) {
    auto time = timeline.get_time();
    auto tuple_count = timeline.get_tuple_count_at(time);
    grounding_table.expire_outdated_groundings(time, tuple_count);
}

std::vector<std::shared_ptr<util::Grounding>>
Algebra::get_groundings(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<util::Grounding>>
Algebra::get_conclusions_step(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<util::Grounding>>
Algebra::get_conclusions_timepoint(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

void Algebra::set_head(bool is_head) {}

bool Algebra::is_head() const { return false; }

uint64_t Algebra::get_window_size() const { return 0; }

FormulaType Algebra::get_type() const { return type; }

std::vector<std::string> const &Algebra::get_predicate_vector() const {
    return predicate_vector;
}

std::map<std::string, size_t> const &Algebra::get_arity_map() const {
    return arity_map;
}

void Algebra::add_child(std::unique_ptr<formula::Formula> child) {}

std::vector<std::unique_ptr<formula::Formula> const *>
Algebra::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    return result;
}

std::vector<std::string> const &Algebra::get_variable_names() const {
    return grounding_table.get_variable_names();
}

int Algebra::get_variable_index(std::string const &variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

size_t Algebra::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

} // namespace laser::formula
