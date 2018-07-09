//
// Created by mike on 6/18/18.
//
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

// constructors & destructor

// getters & setters

size_t GroundingTable::get_size() const {
    return size;
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
    std::list<Grounding> &groundings =
            grounding_map[grounding.get_consideration_time()];
    groundings.push_back(grounding);
    recent_groundings_vector.push_back(grounding);
    size += 1;
}


void GroundingTable::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    for (auto map_iterator = grounding_map.begin();
            map_iterator != grounding_map.end(); ) {
        uint64_t key = map_iterator->first;
        std::list<Grounding> &grounding_list = grounding_map[key];
        for (auto list_iterator = grounding_list.begin();
                list_iterator != grounding_list.end();) {
            Grounding const &grounding = *list_iterator;
            if (grounding.has_expired(current_time, current_tuple_counter)) {
                list_iterator = grounding_list.erase(list_iterator);
                size--;
            } else {
                ++list_iterator;
            }
        }
        if (grounding_list.empty()) {
            map_iterator = grounding_map.erase(map_iterator);
        } else {
            map_iterator++;
        }
    }
    recent_groundings_vector.clear();
}

std::vector<Grounding> GroundingTable::get_all_groundings() const {
    // Merges all lists together
    std::vector<Grounding> all_groundings;
    for (auto const &map_iterator : grounding_map) {
        auto grounding_list = map_iterator.second;
        all_groundings.insert( all_groundings.end(),
                grounding_list.begin(), grounding_list.end() );
    }
    return all_groundings;
}

} // namespace formula
} // namespace laser



