//
// Created by mike on 6/15/18.
//

#include "rule/rule.h"

namespace laser {
namespace rule {


// constructors & destructors
Rule::Rule(formula::Formula *head_formula,
           std::vector<formula::Formula *> formula_vector) : head(head_formula),
                         rule_body(formula_vector) {}

Rule::~Rule() {
    delete head;
}

// getters & setters
formula::Formula &Rule::get_head() {
    return *head;
}

// methods

bool evaluate(long long int current_time,
              long long int current_tuple_counter) {}



} // namespace rule
} // namespace laser
