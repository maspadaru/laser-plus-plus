//
// Created by mike on 6/18/18.
//
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

// constructors & destructor

// getters & setters

unsigned long int GroundingTable::get_number_of_groundings() const {
    return grounding_map.size();
}

unsigned long int GroundingTable::get_number_of_variables() const {
    return variable_name_set.size();
}

std::vector<Grounding> const &GroundingTable::get_recent_groundings_vector() const {
    return recent_groundings_vector;
}

std::set<std::string> const &GroundingTable::get_variable_name_set() const {
    return variable_name_set;
}

// const methods

std::vector<Grounding> const &
GroundingTable::get_groundings(int consideration_time) const {
    return grounding_map.at(consideration_time);
}

// methods

void GroundingTable::add_grounding(Grounding grounding) {
    std::vector<Grounding> groundings =
            grounding_map[grounding.get_consideration_time()];
    groundings.push_back(grounding);
}

void GroundingTable::add_variable_name(std::string variable_name) {
    variable_name_set.insert(variable_name);
}


void GroundingTable::expire_outdated_groundings(long long int current_time,
                                long long int current_tuple_counter) {}

} // namespace formula
} // namespace laser



