#include "formula/grounding_table.h"

namespace laser::formula {

bool GroundingTable::has_recent_groundings() {
    return !recent_groundings_vector.empty();
}

std::vector<grounding_sptr> GroundingTable::get_recent_groundings() {
    std::vector<grounding_sptr> result = std::move(recent_groundings_vector);
    recent_groundings_vector.clear();
    return result;
}

void GroundingTable::add_grounding(grounding_sptr const &grounding) {
    auto &groundings = grounding_map[grounding->get_horizon_time()];
    bool was_inserted = false;
    std::tie(std::ignore, was_inserted) = groundings.insert(grounding);
    if (was_inserted) {
        recent_groundings_vector.push_back(grounding);
    }
}

void GroundingTable::add_grounding_vector(
    std::vector<grounding_sptr> const &grounding_vector) {
    for (auto const &grounding : grounding_vector) {
        add_grounding(grounding);
    }
}

std::vector<grounding_sptr> GroundingTable::get_all_groundings() {
    // Merges all lists together
    std::vector<grounding_sptr> all_groundings;
    for (auto const &map_iterator : grounding_map) {
        auto const &grounding_list = map_iterator.second;
        all_groundings.insert(all_groundings.end(), grounding_list.begin(),
                              grounding_list.end());
    }
    recent_groundings_vector.clear();
    return all_groundings;
}

int GroundingTable::get_variable_index(std::string const &variable_name) const {
    int result = -1;
    auto it = variable_index.find(variable_name);
    if (it != variable_index.end()) {
        result = it->second;
    }
    return result;
}

void GroundingTable::expire_outdated_groundings(uint64_t current_time,
                                                uint64_t tuple_count) {
    // Expire by horizon_time
    uint64_t horizon_time = current_time - 1;
    if (grounding_map.count(horizon_time) > 0) {
        grounding_map.erase(horizon_time);
    }
    // Expire by horizon_count
    for (auto &iterator : grounding_map) {
        grounding_set &set = iterator.second;
        grounding_set toRemove;
        for (auto &grounding : set) {
            auto hc = grounding->get_horizon_count();
            if (hc <= tuple_count) {
                toRemove.insert(grounding);
            } else {
                break;
            }
        }
        for (auto &grounding : toRemove) {
            set.erase(grounding);
        }
    }
    recent_groundings_vector.clear();
}

bool GroundingTable::is_match(grounding_sptr const &left,
                              grounding_sptr const &right) const {
    return left->substitution_equal(*right);
}

void GroundingTable::diamond_update_table(
    std::vector<grounding_sptr> const &new_facts) {
    for (auto &iterator : grounding_map) {
        grounding_set &set = iterator.second;
        grounding_set to_remove;
        for (auto const &old_fact : set) {
            for (auto const &new_fact : new_facts) {
                if (is_match(new_fact, old_fact)) {
                    to_remove.insert(old_fact);
                }
            }
        }
        for (auto &grounding : to_remove) {
            set.erase(grounding);
        }
    }
    add_grounding_vector(new_facts);
}

std::vector<std::string> const &GroundingTable::get_variable_names() const {
    return variable_names;
}

void GroundingTable::set_variable_names(
    std::vector<std::string> variable_names) {
    this->variable_names = std::move(variable_names);
    variable_index.clear();
    for (int i = 0; i < this->variable_names.size(); i++) {
        std::string const &variable_name = this->variable_names.at(i);
        variable_index.try_emplace(variable_name);
        variable_index.at(variable_name) = i;
    }
}

size_t GroundingTable::get_number_of_variables() const {
    return variable_names.size();
}

void GroundingTable::clear() {
    recent_groundings_vector.clear();
    grounding_map.clear();


}

} // namespace laser::formula
