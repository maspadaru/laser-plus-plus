//
// Created by mike on 6/20/18.
//

#include <utility>
#include "program/predicate_information.h"

namespace laser {
namespace program {

// constructors & destructors

PredicateInformation::PredicateInformation(
        std::string predicate,
        bool is_head_of_rule,
        bool is_negated, rule::Rule *rule,
        formula::Formula &formula, std::vector<rule::Rule *> rule_vector)
        : predicate(std::move(predicate)), is_head_of_rule_m(is_head_of_rule),
        is_negated_m(is_negated), rule(rule),
        rule_vector(std::move(rule_vector)) {
    formula_vector.push_back(&formula);
}

PredicateInformation::PredicateInformation(
        std::string predicate,
        bool is_head_of_rule_m,
        bool is_negated_m, rule::Rule *rule,
        std::vector<formula::Formula *>
        formula_vector,
        std::vector<rule::Rule *> rule_vector)
        : predicate(std::move(predicate)),
        is_head_of_rule_m(is_head_of_rule_m),
        is_negated_m(is_negated_m),
        rule(rule),
        formula_vector(std::move(formula_vector)),
        rule_vector(std::move(rule_vector)) {}

// getters & setters

const std::string PredicateInformation::get_predicate() const {
    return predicate;
}

bool PredicateInformation::is_head_of_rule() const {
    return is_head_of_rule_m;
}

bool PredicateInformation::is_negated() const {
    return is_negated_m;
}

rule::Rule *PredicateInformation::get_rule() const {
    return rule;
}

const std::vector<formula::Formula *>
PredicateInformation::get_formula_vector() const {
    return formula_vector;
}

// methods

void PredicateInformation::add_formula(formula::Formula *formula) {
    formula_vector.push_back(formula);
}


} // namespace program
} // namespace laser

