//
// Created by mike on 6/15/18.
//
#ifndef LASER_RULE_RULE_H
#define LASER_RULE_RULE_H

#include "formula/formula.h"
#include "rule/rule_body.h"

namespace laser {
namespace rule {


class Rule {
private:
    formula::Formula &head;
    RuleBody body;


public:

// constructors & destructors
    explicit Rule(
            formula::Formula &head);

    Rule(Rule const &other); // copy constructor
    Rule(Rule &&other) noexcept; // move constructor

    ~Rule();

    Rule &operator=(Rule const &other); // copy assignment
    Rule &operator=(Rule &&other) noexcept; // move assignment

// getters & setters
    formula::Formula &get_head() const;

// methods

    std::string get_head_predicate() const;

//    const std::unordered_map<std::string, std::vector<formula::Formula *>>
//    get_body_variable_map() const;
//
    const std::unordered_map<std::string, std::vector<formula::Formula *>>
    get_body_positive_predicate_map() const;

    const std::unordered_map<std::string, std::vector<formula::Formula *>>
    get_body_negated_predicate_map() const;

    /**
     * Derives new conclusions based on the head of the rule and all groundings
     * that satisfy the body.
     * Also clears previous derivations that no longer hold.
     * @return True if new conclusions have been derived at the current
     * time point and tuple counter
     */
    bool evaluate(
            unsigned long long int current_time,
            unsigned long long int current_tuple_counter);

    void expire_outdated_groundings(
            unsigned long long int current_time,
            unsigned long long int current_tuple_counter);

    void add_body_formula(formula::Formula &formula);

    formula::Formula& get_body_formula(size_t index);

    bool body_has_negated_predicates() const;

};


} // namespace rule
} // namespace laser



#endif //LASER_RULE_RULE_H
