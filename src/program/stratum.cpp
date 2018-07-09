//
// Created by mike on 6/17/18.
//

#include <utility>
#include "program/stratum.h"

namespace laser {
namespace program {

// constructors & destructors

Stratum::~Stratum() {
    predicate_vector.clear();
}

// getters & setters

// methods


void
Stratum::add_head_predicate(
        std::string predicate, formula::Formula &rule_head,
        rule::Rule *rule, std::vector<rule::Rule *> rule_vector) {
    PredicateInformation *pi = new PredicateInformation(
            std::move(predicate), true, false,
            rule, rule_head, std::move(rule_vector));
    predicate_vector.push_back(pi);

}

void Stratum::add_nonhead_predicate(
        std::string predicate, bool is_negated,
        std::vector<formula::Formula *> formula_vector,
        std::vector<rule::Rule *> rule_vector) {

    PredicateInformation *pi = new PredicateInformation(
            std::move(predicate), false,
            is_negated,
            nullptr,
            std::move(formula_vector), std::move(rule_vector));
    predicate_vector.push_back(pi);
}

bool Stratum::evaluate(
        uint64_t time_point, uint64_t tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        new_facts) {
    //TODO
    return false;
}

size_t Stratum::size() {
    return predicate_vector.size();
}

void Stratum::deduplicate() {
    //TODO
}

} // namespace program
} // namespace laser
