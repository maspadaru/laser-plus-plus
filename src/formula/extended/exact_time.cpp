#include "formula/extended/exact_time.h"

namespace laser {
namespace formula {

ExactTime::~ExactTime() { delete child; }

ExactTime::ExactTime(std::string time_variable, Formula *child)
    : time_variable(std::move(time_variable)) {
    this->child = &child->move();
    init();
}

void ExactTime::init() {
    auto variable_names = child->get_variable_names();
    // Add time_variable on the last index of the vector
    variable_names.push_back(time_variable);
    grounding_table.set_variable_names(variable_names);
}

void ExactTime::set_head(bool is_head) { child->set_head(is_head); }

bool ExactTime::is_head() const { return child->is_head(); }

size_t ExactTime::get_time_variable_index() const {
    return grounding_table.get_variable_index(time_variable);
}

Formula &ExactTime::create() const {
    auto result = new ExactTime();
    return *result;
}

Formula &ExactTime::clone() const {
    auto result = new ExactTime(time_variable, &this->child->clone());
    return *result;
}

Formula &ExactTime::move() {
    auto result = new ExactTime(time_variable, &this->child->move());
    return *result;
}

// getters / setters

FormulaType ExactTime::get_type() const { return FormulaType::EXACT_TIME; }

std::vector<std::string> ExactTime::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> ExactTime::get_variable_names() const {
    return grounding_table.get_variable_names();
}

std::vector<std::string> ExactTime::get_full_variable_names() const {
    // Only child variable names should be returned. This method is only useful
    // for writing to output. Since the timevariable will not be writed, it
    // should not be returned.
    auto result = child->get_full_variable_names();
    return result;
}

int ExactTime::get_variable_index(std::string variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

bool ExactTime::is_satisfied() const { return child->is_satisfied(); }

size_t ExactTime::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

std::string ExactTime::debug_string() const { return child->debug_string(); }

void ExactTime::add_child(formula::Formula *child) {}

std::shared_ptr<Grounding>
ExactTime::add_time_variable(util::Timeline const &timeline,
                             Grounding const &grounding) const {
    auto result = grounding.new_constant(get_time_variable_index(),
                                         std::to_string(timeline.get_time()));
    return result;
}

std::shared_ptr<Grounding>
ExactTime::remove_time_variable(Grounding const &grounding) const {
    auto result = grounding.remove_constant(get_time_variable_index());
    return result;
}

std::vector<std::shared_ptr<Grounding>> ExactTime::convert_groundings_head(
    std::vector<std::shared_ptr<Grounding>> const &groundings) const {
    std::vector<std::shared_ptr<Grounding>> result_vector;
    for (auto const &grounding : groundings) {
        size_t timevar_index =
            grounding_table.get_variable_index(time_variable);
        std::string const &horizon_time_string =
            grounding->get_constant(timevar_index);
        uint64_t horizon_time = std::stoull(horizon_time_string);
        auto new_grounding = grounding->new_horizon_time(horizon_time);
        result_vector.push_back(std::move(new_grounding));
    }
    return result_vector;
}

std::vector<std::shared_ptr<Grounding>> ExactTime::convert_groundings_body(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<Grounding>> groundings) const {
    std::vector<std::shared_ptr<Grounding>> result_vector;
    for (auto const &grounding : groundings) {
        auto new_grounding = add_time_variable(timeline, *grounding);
        result_vector.push_back(std::move(new_grounding));
    }
    return result_vector;
}

std::vector<std::shared_ptr<Grounding>> ExactTime::revert_groundings(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<Grounding>> groundings) const {
    std::vector<std::shared_ptr<Grounding>> result_vector;
    for (auto const &grounding : groundings) {
        auto new_grounding = remove_time_variable(*grounding);
        result_vector.push_back(new_grounding);
    }
    return result_vector;
}

std::vector<std::shared_ptr<Grounding>>
ExactTime::get_groundings(util::Timeline timeline) {
    auto grounding_vector = grounding_table.get_all_groundings();
    std::vector<std::shared_ptr<Grounding>> result;
    for (auto &grounding : grounding_vector) {
        std::string const &timevar_string =
            grounding->get_constant(get_time_variable_index());
        uint64_t timevar_value = std::stoull(timevar_string);
        if (timevar_value <= timeline.get_time()) {
            result.push_back(grounding);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Grounding>>
ExactTime::get_conclusions_timepoint(util::Timeline timeline) {
    return timepoint_conclusions;
}

std::vector<std::shared_ptr<Grounding>>
ExactTime::get_conclusions_step(util::Timeline timeline) {
    // 1. Get recent groundings and add them to conclusions_vector or
    // furure_conclusion_map
    std::vector<std::shared_ptr<Grounding>> conclusions_vector;
    auto grounding_vector = grounding_table.get_recent_groundings();
    for (auto &grounding : grounding_vector) {
        std::string const &timevar_string =
            grounding->get_constant(get_time_variable_index());
        uint64_t timevar_value = std::stoull(timevar_string);
        if (timevar_value == timeline.get_time()) {
            conclusions_vector.push_back(grounding);
        } else {
            future_conclusion_map.try_emplace(timevar_value);
            std::set<std::shared_ptr<Grounding>, GroundingFullCompare>
                &map_set = future_conclusion_map[timevar_value];
            map_set.insert(grounding);
        }
    }
    // 2. Add all groundings from future_conclusion_map to conclusions_vector
    std::set<std::shared_ptr<Grounding>, GroundingFullCompare>
        &grounding_set = future_conclusion_map[timeline.get_time()];
    std::copy(grounding_set.begin(), grounding_set.end(),
              std::back_inserter(conclusions_vector));
    future_conclusion_map.erase(timeline.get_time());
    // 3. Conclusions should not contain the time variable
    auto result = revert_groundings(timeline, conclusions_vector);
    // 4. Copy result into timepoint_conclusions before returning
    std::copy(result.begin(), result.end(),
              std::back_inserter(timepoint_conclusions));
    return result;
}

void ExactTime::evaluate_head(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    std::string predicate = get_predicate_vector().at(0);
    auto predicate_facts = facts.at(predicate);
    auto exact_time_groundings =
        convert_groundings_head(predicate_facts);
    grounding_table.add_grounding_vector(exact_time_groundings);
}

void ExactTime::evaluate_body(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    child->evaluate(timeline, facts);
    auto child_conclusions = child->get_groundings(timeline);
    auto exact_time_groundings =
        convert_groundings_body(timeline, child_conclusions);
    grounding_table.add_grounding_vector(exact_time_groundings);
}

bool ExactTime::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    // If the formula is in the head of the rule, we know the child can only
    // be an Atom. So we can ignore the child.
    if (is_head()) {
        evaluate_head(timeline, facts);
    } else {
        evaluate_body(timeline, facts);
    }
    return grounding_table.has_recent_groundings();
}

void ExactTime::expire_outdated_groundings(util::Timeline timeline) {
    timepoint_conclusions.clear();
    child->expire_outdated_groundings(timeline);
    grounding_table.expire_outdated_groundings(timeline.get_min_time(),
                                               timeline.get_tuple_count());
}

} // namespace formula
} // namespace laser
