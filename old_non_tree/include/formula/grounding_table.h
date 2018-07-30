//
// Created by mike on 6/16/18.
//
#ifndef LASER_FORMULA_GROUNDING_TABLE_H
#define LASER_FORMULA_GROUNDING_TABLE_H

#include <string>
#include <unordered_map>
#include <list>

#include "formula/grounding.h"

namespace laser {
namespace formula {

class GroundingTable {
private:
    // TODO should grounding_map value be of type std::set?
    // TODO     std::set -> PRO: unique items; CON: insert complexity O(log n)
    // grounding_map: key: consideration_time
    //        value: Vector of all groundings sharing consideration_time
    //  -- value should contain no duplicates
    std::unordered_map<uint64_t, std::list<Grounding>> grounding_map;
    std::vector<Grounding> recent_groundings_vector;
    size_t size = 0;
public:
// constructors & destructors

// getters & setters

    size_t get_size() const;

    std::vector<Grounding> get_recent_groundings_vector() const;

// const methods

    std::list<Grounding> get_groundings(
            uint64_t consideration_time) const;

    std::vector<Grounding> get_all_groundings() const;

// methods


    void add_grounding(Grounding grounding);

    /**
     * Removes all annotated grounding that have expired due to horizon time
     * or tupple counter
     */
    void expire_outdated_groundings(uint64_t current_time,
                                    uint64_t current_tuple_counter);

};


} // namespace formula
} // namespace laser


#endif // LASER_FORMULA_GROUNDING_TABLE_H
