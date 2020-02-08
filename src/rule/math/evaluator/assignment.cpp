#include "formula/math/assignment.h"

namespace laser::formula {
// private:

void Assignment::init() {
    auto predicate = laser::util::special_predicate::MATH_ASSIGNMENT;
    std::vector<std::string> variable_names;
    variable_names.push_back(this->variable);
    grounding_table.set_variable_names(variable_names);
    this->arity_map.try_emplace(predicate, 1);
    this->predicate_vector.push_back(std::move(predicate));
}

// public :

Assignment::Assignment(std::string variable, std::string value)
    : variable(std::move(variable)), value(std::move(value)) {
    init();
}

bool Assignment::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    auto predicate = laser::util::special_predicate::MATH_ASSIGNMENT;
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



// not interesting:

std::vector<std::shared_ptr<util::Grounding>>
Assignment::get_conclusions_step(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<util::Grounding>>
Assignment::get_conclusions_timepoint(util::Timeline const &timeline) {
    return grounding_table.get_all_groundings();
}

void Assignment::set_head(bool is_head) {}

bool Assignment::is_head() const { return false; }

uint64_t Assignment::get_window_size() const { return 0; }

FormulaType Assignment::get_type() const { return type; }

std::vector<std::string> const &Assignment::get_predicate_vector() const {
    return predicate_vector;
}

std::map<std::string, size_t> const &Assignment::get_arity_map() const {
    return arity_map;
}

void Assignment::add_child(std::unique_ptr<formula::Formula> child) {}

std::vector<std::unique_ptr<formula::Formula> const *>
Assignment::get_children() const {
    std::vector<std::unique_ptr<formula::Formula> const *> result;
    return result;
}

std::vector<std::string> const &Assignment::get_variable_names() const {
    return grounding_table.get_variable_names();
}

int Assignment::get_variable_index(std::string const &variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

size_t Assignment::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

} // namespace laser::formula
