//
// Created by mike on 6/17/18.
//

#ifndef LASER_PROGRAM_STRATUM_H
#define LASER_PROGRAM_STRATUM_H

#include <vector>

#include "formula/formula.h"
#include "program/predicate_information.h"

namespace laser {
namespace program {


class Stratum {
private:

    std::vector<PredicateInformation *> predicate_vector;

public:

// constructors & destructors

    ~Stratum();

// getters & setters

// const methods

// methods

    void add_head_predicate(
            std::string predicate,
            formula::Formula &rule_head,
            rule::Rule *rule, std::vector<rule::Rule *> rule_vector);

    void add_nonhead_predicate(
            std::string predicate,
            bool is_negated,
            std::vector<formula::Formula *> formula_vector,
            std::vector<rule::Rule *> rule_vector);

    bool evaluate(
            uint64_t time_point, uint64_t tuple_counter,
            std::unordered_map<std::string, std::vector<formula::Formula *>>
            new_facts);

    size_t size();

    void deduplicate();

};

} // namespace program
} // namespace laser


#endif // LASER_PROGRAM_STRATUM_H
