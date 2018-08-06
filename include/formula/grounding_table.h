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
    std::vector<std::string> variable_names;
    std::unordered_map<std::string, int> variable_index;
    size_t size = 0;
public:

// getters & setters

    size_t get_size() const;

    std::vector<Grounding> get_recent_groundings_vector() const;

    std::vector<std::string> get_variable_names() const;

    void set_variable_names(std::vector<std::string> variable_names);


// methods
    
    void add_variable_name(std::string const& variable_name);

    size_t get_number_of_variables() const;

    int get_variable_index(std::string variable_name) const;

    std::list<Grounding> get_groundings(
            uint64_t consideration_time) const;

    std::vector<Grounding> get_all_groundings() const;

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
