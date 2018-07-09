//
// Created by mike on 6/18/18.
//
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

// constructors & destructor

// getters & setters

size_t GroundingTable::get_number_of_groundings() const {
    return grounding_map.size();
}

std::vector<Grounding> GroundingTable::get_recent_groundings_vector() const {
    return recent_groundings_vector;
}

// const methods

std::list<Grounding>
GroundingTable::get_groundings(uint64_t consideration_time) const {
    return grounding_map.at(consideration_time);
}

// methods

void GroundingTable::add_grounding(Grounding grounding) {
    std::list<Grounding> groundings =
            grounding_map[grounding.get_consideration_time()];
    groundings.push_back(grounding);
}

void GroundingTable::expire_outdated_groundings(uint64_t current_time,
                                uint64_t current_tuple_counter) {
    // TODO
}

} // namespace formula
} // namespace laser



