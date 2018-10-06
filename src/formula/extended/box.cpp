#include "formula/extended/box.h"

namespace laser {
namespace formula {

Box::~Box() { delete child; }

Box::Box(Formula *child) { this->child = &child->move(); }

Formula &Box::create() const {
    auto result = new Box();
    return *result;
}

Formula &Box::clone() const {
    auto result = new Box(&this->child->clone());
    return *result;
}

Formula &Box::move() {
    auto result = new Box(&this->child->move());
    return *result;
}

// getters / setters

FormulaType Box::get_type() const { return FormulaType::BOX; }

std::vector<std::string> Box::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Box::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Box::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Box::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Box::is_satisfied() const { return child->is_satisfied(); }

size_t Box::get_number_of_variables() const {
    return child->get_number_of_variables();
}

std::string Box::debug_string() const { return child->debug_string(); }

void Box::add_child(formula::Formula *child) {}

void Box::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
    grounding_table.expire_outdated_groundings(timeline.get_min_time(),
                                               timeline.get_min_tuple_count());
}

bool Box::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    bool result = child->evaluate(timeline, facts);
    auto child_facts = child->get_groundings(timeline);
    grounding_table.add_grounding_vector(child_facts);
    return result;
}


std::vector<Grounding> Box::get_groundings(util::Timeline timeline) const {
     auto grounding_vector = compute_valid_groundings(timeline);
     for (auto &grounding : grounding_vector) {
         grounding.set_consideration_time(timeline.get_time());
         grounding.set_horizon_time(timeline.get_time());
     } 
     return grounding_vector;
}

void Box::build_maps(
        std::unordered_map<std::string, std::set<uint64_t>> &timepoint_map,
        std::unordered_map<std::string, Grounding> &grounding_map) const {
    for (auto const &grounding : grounding_table.get_all_groundings()) {
        std::string key = grounding.compute_hash();
        timepoint_map.try_emplace(key);
        std::set<uint64_t> &timepoint_set = timepoint_map[key]; 
        timepoint_set.insert(grounding.get_consideration_time());
        grounding_map.try_emplace(key, grounding);
    }
}


std::vector<Grounding> Box::compute_valid_groundings(util::Timeline timeline) const {
    std::unordered_map<std::string, std::set<uint64_t>> timepoint_map;
    std::unordered_map<std::string, Grounding> grounding_map;
    std::vector<Grounding> result;

    // select groundings for which the size of the array in timepoint_map ==
    // EITHER: the number of timepoints in the timeline 
    // OR: timeline.get_time() - timepoint.get_min_time()
    // Not sure which is correct
    // for each grounding also set time values to NOW


    return result;
}

} // namespace formula
} // namespace laser
