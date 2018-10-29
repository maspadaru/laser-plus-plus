//
// Created by mike on 6/18/18.
//
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

// constructors & destructor

// getters & setters

size_t GroundingTable::get_size() const { return size; }

std::vector<Grounding> GroundingTable::get_recent_groundings_vector() const {
    return recent_groundings_vector;
}

// const methods

std::list<Grounding>
GroundingTable::get_groundings(uint64_t consideration_time) const {
    return grounding_map.at(consideration_time);
}

// methods

void GroundingTable::add_grounding(Grounding const &grounding) {
    std::list<Grounding> &groundings =
        grounding_map[grounding.get_consideration_time()];
    groundings.push_back(grounding);
    recent_groundings_vector.push_back(grounding);
    size += 1;
}
    
void GroundingTable::add_grounding_vector(std::vector<Grounding> const &grounding_vector) {
    for (auto const &grounding : grounding_vector) {
        add_grounding(grounding);
    }
}

std::vector<Grounding> GroundingTable::get_all_groundings() const {
    // Merges all lists together
    std::vector<Grounding> all_groundings;
    for (auto const &map_iterator : grounding_map) {
        auto grounding_list = map_iterator.second;
        all_groundings.insert(all_groundings.end(), grounding_list.begin(),
                              grounding_list.end());
    }
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
    size_t new_size = 0;
    for (auto &map_iterator : grounding_map) {
        auto &grounding_list = map_iterator.second;
        grounding_list.remove_if([current_time, current_tuple_counter](
                                     Grounding const &grounding) {
            return grounding.has_expired(current_time, current_tuple_counter);
        });
        new_size += grounding_list.size();
    }
    recent_groundings_vector.clear();
    size = new_size;
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
