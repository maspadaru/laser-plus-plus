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
    auto result = child->get_full_variable_names();
    // Add time_variable on the last index of the vector
    result.push_back(time_variable);
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

Grounding ExactTime::add_time_variable(util::Timeline timeline,
                                       Grounding grounding) const {
    auto result = grounding.add_constant(get_time_variable_index(), 
            std::to_string(timeline.get_time()));
    return result;
}

std::vector<Grounding>
ExactTime::convert_groundings(util::Timeline timeline,
                              std::vector<Grounding> child_groundings) const {
    std::vector<Grounding> result_vector;
    for (auto const &child_grounding : child_groundings) {
        auto new_grounding = add_time_variable(timeline, child_grounding);
        result_vector.push_back(new_grounding);
    }
    return result_vector;
}

std::vector<Grounding> ExactTime::get_groundings(util::Timeline timeline) {
    auto grounding_vector = grounding_table.get_all_groundings();
    std::vector<Grounding> result;
    if (!grounding_vector.empty()) {
        for (auto &grounding : grounding_vector) {
            std::string timevar_string =
                grounding.get_substitution(get_time_variable_index());
            uint64_t timevar_value = std::stoull(timevar_string);
            if (timevar_value <= timeline.get_time()) {
                result.push_back(grounding);
            }
        }
    }
    return result;
}

std::vector<Grounding> ExactTime::get_conclusions(util::Timeline timeline) {
    // 1. Get recent groundings and add them to result or furure_conclusion_map
    std::vector<Grounding> result;
    auto grounding_vector = grounding_table.get_recent_groundings();
    if (!grounding_vector.empty()) {
        for (auto &grounding : grounding_vector) {
            std::string timevar_string =
                grounding.get_substitution(get_time_variable_index());
            uint64_t timevar_value = std::stoull(timevar_string);
            if (timevar_value == timeline.get_time()) {
                result.push_back(grounding);
            } else {
                future_conclusion_map.try_emplace(timevar_value);
                std::set<Grounding> &map_set = future_conclusion_map[timevar_value];
                map_set.insert(grounding);
            }
        }
    }
    // 2. Add all groundings from future_conclusion_map to result
    std::set<Grounding> &grounding_set = future_conclusion_map[timeline.get_time()];
    std::copy(grounding_set.begin(), grounding_set.end(), std::back_inserter(result));
    future_conclusion_map.erase(timeline.get_time());
    return result;
}

bool ExactTime::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    child->evaluate(timeline, facts);
    auto child_conclusions = child->get_groundings(timeline);
    auto exact_time_groundings =
        convert_groundings(timeline, child_conclusions);
    grounding_table.add_grounding_vector(exact_time_groundings);
    return grounding_table.has_recent_groundings();
}

void ExactTime::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
    grounding_table.expire_outdated_groundings(timeline.get_min_time(),
                                               timeline.get_min_tuple_count());
}

} // namespace formula
} // namespace laser
