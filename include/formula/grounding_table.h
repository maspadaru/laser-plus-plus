//
// Created by mike on 6/16/18.
//
#ifndef LASER_FORMULA_GROUNDING_TABLE_H
#define LASER_FORMULA_GROUNDING_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "formula/grounding.h"

namespace laser {
namespace formula {

class GroundingTable {
private:
    // TODO should grounding_map value be of type std::set?
    // TODO     std::set -> PRO: unique items; CON: insert complexity O(log n)
    // grounding_map: key: consideration_time
    //        value: Vector of all groundings sharing consid_time
    //  -- value should contain no duplicates
    std::unordered_map<int, std::vector<Grounding>> grounding_map;
    std::set<std::string> variable_name_set;
    std::vector<Grounding> recent_groundings_vector;
public:
// constructors & destructors

// getters & setters

    unsigned long int get_number_of_groundings() const;

    unsigned long int get_number_of_variables() const;

    std::set<std::string> const &get_variable_name_set() const;

    std::vector<Grounding> const &get_recent_groundings_vector() const;

// const methods

    std::vector<Grounding> const &get_groundings(
            int consideration_time) const;

// methods

    void add_grounding(Grounding grounding);

    void add_variable_name(std::string variable_name);

    /**
     * Removes all annotated grounding that have expired due to horison time
     * or tupple counter
     */
    void expire_outdated_groundings(long long int current_time,
                                    long long int current_tuple_counter);

};


} // namespace formula
} // namespace laser


#endif // LASER_FORMULA_GROUNDING_TABLE_H
