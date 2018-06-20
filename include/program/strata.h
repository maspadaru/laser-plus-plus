//
// Created by mike on 6/20/18.
//

#ifndef LASER_PROGRAM_STRATA_H
#define LASER_PROGRAM_STRATA_H

#include "stratum.h"

namespace laser {
namespace program {

/**
 *
 */
class Strata {
private:
    bool is_stratified = false;
    std::vector<Stratum> stratum_vector;

    /* key - predicate /
     * value - vector containing all rules that have key as predicate in the
     * head formula of the rule.
     */
    std::unordered_map<std::string, std::vector<rule::Rule *>>
            head_predicate_map;

// methods

    void
    map_head_predicate_to_rule(std::vector<rule::Rule *> const &rule_vector);

    /**
     *
     * @param stratum_index
     * @param predicate
     * @param rule
     * @param temp_stratum_vector OUTPUT parameter. It's value is updated at each
     * recursive call
     * @return
     */
    void follow_rule_dependencies(
            size_t stratum_index,
            bool is_negated,
            rule::Rule *rule,
            std::vector<laser::program::Stratum> *temp_stratum_vector);
    /**
     *
     * @param stratum_index
     * @param rule
     * @param temp_stratum_vector OUTPUT parameter. It's value is updated at each
     * recursive call
     * @return
     */
    void check_rule_dependencies(size_t stratum_index, rule::Rule *rule,
                                         std::vector<Stratum> *temp_stratum_vector);

    void deduplicate(size_t stratum_index);

public:

// constructors & destructors
    Strata() = default;

    ~Strata() = default;

// getters & setters

// methods
    bool evaluate(long long int time_point, long long int tuple_counter,
              std::unordered_map<std::string, std::vector<formula::Formula *>>
              new_facts);

    void stratify(std::vector<rule::Rule *> rule_vector);
};

} // namespace program
} // namespace laser

#endif //LASER_PROGRAM_STRATA_H
