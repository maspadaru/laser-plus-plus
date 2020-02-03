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

    grounding_set recent_groundings_set;
    std::vector<std::string> variable_names;
    std::unordered_map<std::string, int> variable_index;

    grounding_sptr merge(grounding_sptr const &left,
                         grounding_sptr const &right) const;

    bool is_match(grounding_sptr const &left,
                  grounding_sptr const &right) const;

    void update_map(uint64_t current_time);

  public:
    std::vector<std::string> const &get_variable_names() const;

    void set_variable_names(std::vector<std::string> variable_names);

    bool has_recent_groundings();

    size_t get_number_of_variables() const;

    int get_variable_index(std::string const &variable_name) const;

    void add_grounding(grounding_sptr const &grounding);

    void
    add_grounding_vector(std::vector<grounding_sptr> const &grounding_vector);

    std::vector<grounding_sptr> get_old_groundings() const;

    std::vector<grounding_sptr> get_new_groundings() const;

    std::vector<grounding_sptr> get_groundings_at(uint64_t time) const;

    void new_step(uint64_t current_time);

    /**
     * Removes all annotated grounding that have expired due to horizon time
     * or tupple counter
     */
    void expire_outdated_groundings(uint64_t expiration_time,
                                    uint64_t expiration_tuple_count);
};

} // namespace laser::formula

#endif // LASER_FORMULA_GROUNDING_TABLE_H
