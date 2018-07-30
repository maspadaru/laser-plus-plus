//
// Created by mike on 6/15/18.
//
#ifndef LASER_RULE_RULE_H
#define LASER_RULE_RULE_H

#include <iostream>

#include "formula/formula.h"

namespace laser {
namespace rule {


class Rule {
private:
    formula::Formula &head;
    formula::Formula &body;


public:

// constructors & destructors

    explicit Rule(
            formula::Formula *head_formula,
            formula::Formula *body_formula);

    Rule(Rule const &other); // copy constructor
    Rule(Rule &&other) noexcept; // move constructor

    ~Rule();

    Rule &operator=(Rule const &other); // copy assignment
    Rule &operator=(Rule &&other) noexcept; // move assignment

// getters & setters

// methods

    /**
     * Derives new conclusions based on the head of the rule and all groundings
     * that satisfy the body.
     * Also clears previous derivations that no longer hold.
     * @return True if new conclusions have been derived at the current
     * time point and tuple counter
     */
    bool evaluate(
            uint64_t current_time,
            uint64_t current_tuple_counter,
            std::unordered_map<std::string, std::vector<formula::Formula *>>
            facts
    );

    void expire_outdated_groundings(
            uint64_t current_time,
            uint64_t current_tuple_counter);

    void debug_print() const;

};


} // namespace rule
} // namespace laser



#endif //LASER_RULE_RULE_H
