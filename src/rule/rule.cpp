//
// Created by mike on 6/15/18.
//

#include <utility>
#include "rule/rule.h"

namespace laser {
namespace rule {


// constructors & destructors
Rule::Rule(
        formula::Formula *head_formula,
        std::vector<formula::Formula *> formula_vector) : head(head_formula),
        body(std::move(formula_vector)) {}

Rule::~Rule() {
    delete head;
}

Rule::Rule(Rule const &other) : body(other.body) {
    this->head = other.head->clone();
}

Rule::Rule(Rule &&other) noexcept  : body(std::move(other.body)) {
    this->head = other.head->clone();
};

Rule &Rule::operator=(Rule const &other) {
    this->head = other.head->clone();
    this->body = RuleBody(other.body);
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->head = other.head->clone();
    this->body = std::move(other.body);
    return *this;
}

// getters & setters
formula::Formula *Rule::get_head() {
    return head;
}

// methods

std::string Rule::get_head_predicate() const {
    return head->get_predicate();
}

bool Rule::evaluate(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {}

const std::unordered_map<std::string, std::vector<formula::Formula *>>
Rule::get_body_variable_map() const {
    return body.get_variable_map();
};

const std::unordered_map<std::string, std::vector<formula::Formula *>>
Rule::get_body_positive_predicate_map() const {
    return body.get_positive_predicate_map();
};

const std::unordered_map<std::string, std::vector<formula::Formula *>>
Rule::get_body_negated_predicate_map() const {
    return body.get_negated_predicate_map();
}

void Rule::expire_outdated_groundings(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {
    head->expire_outdated_groundings(current_time, current_tuple_counter);
    body.expire_outdated_groundings(current_time, current_tuple_counter);

}


} // namespace rule
} // namespace laser
