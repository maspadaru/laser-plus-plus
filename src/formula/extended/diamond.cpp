#include "formula/extended/diamond.h"

namespace laser {
namespace formula {

Diamond::~Diamond() { delete child; }

Diamond::Diamond(Formula *child) { this->child = &child->move(); }

Formula &Diamond::create() const {
    auto result = new Diamond();
    return *result;
}

Formula &Diamond::clone() const {
    auto result = new Diamond(&this->child->clone());
    return *result;
}

Formula &Diamond::move() {
    auto result = new Diamond(&this->child->move());
    return *result;
}

// getters / setters

void Diamond::set_head(bool is_head) { is_head_m = is_head; }

bool Diamond::is_head() const { return is_head_m; }

FormulaType Diamond::get_type() const { return FormulaType::DIAMOND; }

std::vector<std::string> Diamond::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Diamond::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Diamond::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Diamond::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Diamond::is_satisfied() const { return child->is_satisfied(); }

size_t Diamond::get_number_of_variables() const {
    return child->get_number_of_variables();
}

std::string Diamond::debug_string() const { return child->debug_string(); }

void Diamond::add_child(formula::Formula *child) {}

std::vector<std::shared_ptr<Grounding>>
Diamond::get_groundings(util::Timeline timeline) {
    std::vector<std::shared_ptr<Grounding>> all_groundings;
    for (auto const &map_iterator : grounding_map) {
        auto grounding_list = map_iterator.second;
        all_groundings.insert(all_groundings.end(), grounding_list.begin(),
                              grounding_list.end());
    }
    recent_groundings_vector.clear();
    return all_groundings;
}

std::vector<std::shared_ptr<Grounding>>
Diamond::get_conclusions_timepoint(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::vector<std::shared_ptr<Grounding>>
Diamond::get_conclusions_step(util::Timeline timeline) {
    std::vector<std::shared_ptr<Grounding>> result = recent_groundings_vector;
    recent_groundings_vector.clear();
    return result;
}

bool Diamond::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    bool result = child->evaluate(timeline, facts);
    auto child_facts = child->get_groundings(timeline);
    add_grounding_vector(child_facts);
    return result;
}

void Diamond::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
    expire_from_map(timeline.get_min_time(), timeline.get_min_tuple_count());
}

void Diamond::expire_from_map(uint64_t current_time,
                              uint64_t current_tuple_counter) {
    uint64_t horizon_time = current_time - 1;
    if (grounding_map.count(horizon_time) > 0) {
        grounding_map.erase(horizon_time);
    }
    recent_groundings_vector.clear();
    /* TODO expire also by hourizon_counter
     * Not sure if it's more efficient to create a new map based on ht, or
     * a nested hashtable, or just to itterate over all keys
     */
}

void Diamond::add_grounding(std::shared_ptr<Grounding> const &grounding) {
    auto horizon_time = grounding->get_horizon_time();
    auto &groundings = grounding_map[horizon_time];
    // Setting horizon time to infinite before before inserting
    auto diamond_grounding =
        grounding->new_horizon_time(util::Timeline::INFINITE_TIME);
    groundings.push_back(diamond_grounding);
    recent_groundings_vector.push_back(diamond_grounding);
}

void Diamond::add_grounding_vector(
    std::vector<std::shared_ptr<Grounding>> const &grounding_vector) {
    for (auto const &grounding : grounding_vector) {
        add_grounding(grounding);
    }
}

} // namespace formula
} // namespace laser
