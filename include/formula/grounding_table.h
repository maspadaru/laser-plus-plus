#ifndef LASER_FORMULA_GROUNDING_TABLE_H
#define LASER_FORMULA_GROUNDING_TABLE_H

#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "util/grounding.h"

namespace laser::formula {

using grounding_sptr = std::shared_ptr<util::Grounding>;
using grounding_set = std::set<grounding_sptr, util::GroundingFullCompare>;

class GroundingTable {
  private:
    // grounding_map: key: horizon_time
    //        value: Set of all groundings sharing consideration_time
    std::unordered_map<uint64_t, grounding_set> grounding_map;

    std::vector<grounding_sptr> recent_groundings_vector;
    std::vector<std::string> variable_names;
    std::unordered_map<std::string, int> variable_index;

    bool is_match(grounding_sptr const &left,
                              grounding_sptr const &right) const; 

  public:

    std::vector<std::string> const &get_variable_names() const;

    void set_variable_names(std::vector<std::string> variable_names);

    /** Returns the contents of recent groundings set as a vector, and clears it
     */
    std::vector<grounding_sptr> get_recent_groundings();

    bool has_recent_groundings();

    size_t get_number_of_variables() const;

    int get_variable_index(std::string const &variable_name) const;

    std::vector<grounding_sptr> get_all_groundings();

    void add_grounding(grounding_sptr const &grounding);

    void
    add_grounding_vector(std::vector<grounding_sptr> const &grounding_vector);

    /**
     * Removes all annotated grounding that have expired due to horizon time
     * or tupple counter
     */
    void expire_outdated_groundings(uint64_t expiration_time,
                                    uint64_t expiration_tuple_count);

    void diamond_update_table(std::vector<grounding_sptr> const &new_facts);
};

} // namespace laser::formula

#endif // LASER_FORMULA_GROUNDING_TABLE_H
