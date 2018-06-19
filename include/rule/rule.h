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
    formula::Formula *head;
    RuleBody rule_body;


public:
// constructors & destructors
    explicit Rule(formula::Formula *head,
                  std::vector<formula::Formula *> formula_vector);

    ~Rule();

// getters & setters
    formula::Formula &get_head();

// const methods

// methods

    /**
     * Derives new conclusions based on the head of the rule and all groundings
     * that satisfy the body.
     * Also clears previous derivations that no longer hold.
     * @return True if new conclusions have been derived at the current
     * time point and tuple counter
     */
    bool evaluate(long long int current_time,
                  long long int current_tuple_counter);

};


} // namespace rule
} // namespace laser



#endif //LASER_RULE_RULE_H
