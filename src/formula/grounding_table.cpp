#include "formula/grounding_table.h"

namespace laser::formula {

bool GroundingTable::has_recent_groundings() {
    return !recent_groundings_set.empty();
}

std::vector<grounding_sptr> GroundingTable::get_old_groundings() const {
    std::vector<grounding_sptr> result;
    for (auto const &map_iterator : grounding_map) {
        auto const &grounding_list = map_iterator.second;
        result.insert(result.end(), grounding_list.begin(),
                      grounding_list.end());
    }
    return result;
}

std::vector<grounding_sptr> GroundingTable::get_new_groundings() const {
    std::vector<grounding_sptr> result;
    std::move(recent_groundings_set.begin(), recent_groundings_set.end(),
              std::back_inserter(result));
    return result;
}

std::vector<grounding_sptr>
GroundingTable::get_groundings_at(uint64_t time) const {
    std::vector<grounding_sptr> result;
    if (grounding_map.count(time) > 0) {
        auto const &facts = grounding_map.at(time);
        std::copy(facts.begin(), facts.end(), std::back_inserter(result));
    }
    return result;
}

void GroundingTable::new_step(uint64_t current_time) {
    update_map(current_time);
    recent_groundings_set.clear();
}

void GroundingTable::add_grounding(grounding_sptr const &grounding) {
    recent_groundings_set.insert(grounding);
}

void GroundingTable::add_grounding_vector(
    std::vector<grounding_sptr> const &grounding_vector) {
    std::copy(
        grounding_vector.begin(), grounding_vector.end(),
        std::inserter(recent_groundings_set, recent_groundings_set.end()));
}

int GroundingTable::get_variable_index(std::string const &variable_name) const {
    int result = -1;
    auto it = variable_index.find(variable_name);
    if (it != variable_index.end()) {
        result = it->second;
    }
    return result;
}

// void GroundingTable::expire_outdated_groundings(uint64_t current_time,
// uint64_t tuple_count) {
//// Expire by horizon_time
// uint64_t horizon_time = current_time - 1;
// if (grounding_map.count(horizon_time) > 0) {
// grounding_map.erase(horizon_time);
//}
//// Expire by horizon_count
// for (auto &iterator : grounding_map) {
// grounding_set  &set = iterator.second;
// grounding_set toRemove;
// for (auto &grounding : set) {
// auto hc = grounding->get_horizon_count();
// if (hc <= tuple_count) {
// toRemove.insert(grounding);
//} else {
// break;
//}
//}
// for (auto &grounding : toRemove) {
// set.erase(grounding);
//}
//}
// recent_groundings_vector.clear();
//}

grounding_sptr GroundingTable::merge(grounding_sptr const &left,
                                     grounding_sptr const &right) const {
    grounding_sptr result;
    return result;
}

bool GroundingTable::is_match(grounding_sptr const &left,
                              grounding_sptr const &right) const {
    return false;
}

void GroundingTable::update_map(uint64_t current_time) {
    grounding_set new_set = grounding_map[current_time];
    grounding_set to_remove;
    auto previous_time = current_time - 1;
    if (previous_time > 0 && grounding_map.count(previous_time) > 0) {
        auto &previous_set = grounding_map.at(previous_time);
        for (auto const &recent_fact : recent_groundings_set) {
            bool found = false;
            for (auto const &previous_fact : previous_set) {
                if (!found && is_match(recent_fact, previous_fact)) {
                    auto new_fact = merge(recent_fact, previous_fact);
                    to_remove.insert(previous_fact);
                    new_set.insert(std::move(new_fact));
                    found = true;
                }
            }
            if (!found) {
                new_set.insert(recent_fact);
            }
        }
        for (auto &grounding : to_remove) {
            previous_set.erase(grounding);
        }
    } else {
        new_set.insert(recent_groundings_set.begin(),
                       recent_groundings_set.end());
    }
}

void GroundingTable::expire_outdated_groundings(uint64_t current_time,
                                                uint64_t tuple_count) {
    // Expire by horizon_time and horizon_count
    for (auto &iterator : grounding_map) {
        grounding_set &set = iterator.second;
        grounding_set to_remove;
        for (auto &grounding : set) {
            auto hc = grounding->get_horizon_count();
            auto ht = grounding->get_horizon_time();
            if (ht < current_time || hc <= tuple_count) {
                to_remove.insert(grounding);
            }
        }
        for (auto &grounding : to_remove) {
            set.erase(grounding);
        }
    }
    update_map(current_time);
    recent_groundings_set.clear();
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

} // namespace laser::formula
