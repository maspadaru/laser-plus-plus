#include "formula/grounding_table.h"

namespace laser::formula {

size_t GroundingTable::get_size() const { return size; }

bool GroundingTable::has_recent_groundings() {
    return !recent_groundings_vector.empty();
}

std::vector<std::shared_ptr<util::Grounding>>
GroundingTable::get_recent_groundings() {
    std::vector<std::shared_ptr<util::Grounding>> result =
        std::move(recent_groundings_vector);
    recent_groundings_vector.clear();
    return result;
}

void GroundingTable::add_grounding(
    std::shared_ptr<util::Grounding> const &grounding) {
    auto &groundings = grounding_map[grounding->get_horizon_time()];
    bool was_inserted = false;
    std::tie(std::ignore, was_inserted) = groundings.insert(grounding);
    if (was_inserted) {
        recent_groundings_vector.push_back(grounding);
        size += 1;
    }
}

void GroundingTable::add_grounding_vector(
    std::vector<std::shared_ptr<util::Grounding>> const &grounding_vector) {
    for (auto const &grounding : grounding_vector) {
        add_grounding(grounding);
    }
}

std::vector<std::shared_ptr<util::Grounding>> GroundingTable::get_all_groundings() {
    // Merges all lists together
    std::vector<std::shared_ptr<util::Grounding>> all_groundings;
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
    if (variable_index.count(variable_name) > 0) {
        result = variable_index.at(variable_name);
    }
    return result;
}

void GroundingTable::expire_outdated_groundings(uint64_t current_time,
                                                uint64_t tuple_count) {
    // Expire by horizon_time
    uint64_t horizon_time = current_time - 1;
    if (grounding_map.count(horizon_time) > 0) {
        size -= grounding_map[horizon_time].size();
        grounding_map.erase(horizon_time);
    }
    // Expire by horizon_count
    for (auto &iterator : grounding_map) {
        std::set<std::shared_ptr<util::Grounding>, util::GroundingFullCompare> &set =
            iterator.second;
        std::set<std::shared_ptr<util::Grounding>, util::GroundingFullCompare> toRemove;
        for (auto &grounding : set) {
            auto hc = grounding->get_horizon_count();
            if (hc <= tuple_count) {
                toRemove.insert(grounding);
                // set.erase(grounding);
                // size--;
            } else {
                // Sets in map are ordered with lowest horizon_counter
                // first.
                break;
            }
        }
        size -= toRemove.size();
        for (auto &grounding: toRemove) {
            set.erase(grounding);
        }
    }
    recent_groundings_vector.clear();
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
