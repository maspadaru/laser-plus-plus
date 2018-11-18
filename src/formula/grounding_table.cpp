//
// Created by mike on 6/18/18.
//
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

size_t GroundingTable::get_size() const { return size; }

bool GroundingTable::has_recent_groundings() {
    return !recent_groundings_set.empty();
}

std::vector<Grounding> GroundingTable::get_recent_groundings() {
    std::vector<Grounding> result(recent_groundings_set.size());
    std::move(recent_groundings_set.begin(), recent_groundings_set.end(), 
            std::back_inserter(result));
    recent_groundings_set.clear(); // should not be necesarry
    return result;
}

void GroundingTable::add_grounding(Grounding const &grounding) {
    std::set<Grounding> &groundings =
        grounding_map[grounding.get_horizon_time()];
    groundings.insert(grounding);
    recent_groundings_set.insert(grounding);
    size += 1;
}
    
void GroundingTable::add_grounding_vector(std::vector<Grounding> const &grounding_vector) {
    for (auto const &grounding : grounding_vector) {
        add_grounding(grounding);
    }
}

std::vector<Grounding> GroundingTable::get_all_groundings() {
    // Merges all lists together
    std::vector<Grounding> all_groundings;
    for (auto const &map_iterator : grounding_map) {
        auto grounding_list = map_iterator.second;
        all_groundings.insert(all_groundings.end(), grounding_list.begin(),
                              grounding_list.end());
    }
    recent_groundings_set.clear();
    return all_groundings;
}

int GroundingTable::get_variable_index(std::string const &variable_name) const {
    int result = -1;
    if (variable_index.count(variable_name) > 0) { 
        result = variable_index.at(variable_name);
    }
    return result;
}

void GroundingTable::expire_outdated_groundings(
    uint64_t current_time, uint64_t current_tuple_counter) {
    uint64_t horizon_time = current_time - 1;
    if (grounding_map.count(horizon_time) > 0) {
        size -= grounding_map[horizon_time].size();
        grounding_map.erase(horizon_time);
    }
    recent_groundings_set.clear();
    /* TODO expire also by hourizon_counter
     * Not sure if it's more efficient to create a new map based on ht, or 
     * a nested hashtable, or just to itterate over all keys
     */
}

std::vector<std::string> GroundingTable::get_variable_names() const {
    return variable_names;
}

void GroundingTable::set_variable_names(
    std::vector<std::string> variable_names) {
    this->variable_names = std::move(variable_names);
    variable_index.clear();
    for (int i = 0; i < this->variable_names.size(); i++) {
        std::string variable_name = this->variable_names.at(i);
        variable_index.try_emplace(variable_name);
        variable_index.at(variable_name) = i;
    }
}

size_t GroundingTable::get_number_of_variables() const {
    return variable_names.size();
}

void GroundingTable::add_variable_name(std::string const &variable_name) {
    this->variable_names.push_back(variable_name);
}

template <typename T>
void GroundingTable::debug_print(std::string const &message,
                                 T const &value) const {
    std::cerr << "GroundingTable -> "
              << "";
    std::cerr << message << " : " << value << std::endl;
}

} // namespace formula
} // namespace laser
