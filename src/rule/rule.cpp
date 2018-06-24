//
// Created by mike on 6/15/18.
//

#include <utility>
#include <iostream>
#include "rule/rule.h"

namespace laser {
namespace rule {


// constructors & destructors



Rule::Rule(
        formula::Formula *head_formula,
        std::vector<formula::Formula *> body_vector) :
        head(head_formula->move()),
        body(std::move(body_vector)) {
    std::cerr << "Rule-CREATE -> head: " << head.get_predicate() << "; body size = "
              << body.get_size() << std::endl;

    std::cerr << "Body predicates: ";
    for(size_t i = 0; i < body.get_size(); i++) {
        auto &formula = body.get_formula(i);
        std::cerr << formula.get_predicate() << ", ";
    }
    std::cerr << "// Rule " << std::endl;
}

Rule::~Rule() {
    std::cerr << "~Rule() -> ";
    delete &head;
    std::cerr << "// Rule " << std::endl;
}

Rule::Rule(Rule const &other) : head(other.head.clone()), body(other.body) {
    std::cerr << "Rule-COPY -> head: " << head.get_predicate() << "; body size = "
              << body.get_size() << std::endl;
    std::cerr << "// Rule " << std::endl;
}

Rule::Rule(Rule &&other) noexcept  : head(other.head.move()),
        body(std::move(other.body)) {
    std::cerr << "Rule-MOVE -> head: " << head.get_predicate() << "; body size = "
              << body.get_size() << std::endl;
    std::cerr << "// Rule " << std::endl;
}

Rule &Rule::operator=(Rule const &other) {
    std::cerr << "Rule: COPY operator called" << std::endl;
    this->head = other.head.clone();
    this->body = RuleBody(other.body);
    std::cerr << "// Rule " << std::endl;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    std::cerr << "Rule: MOVE operator called" << std::endl;
    this->head = other.head.move();
    this->body = std::move(other.body);
    std::cerr << "// Rule " << std::endl;
    return *this;
}

// getters & setters
formula::Formula &Rule::get_head() const {
    return head;
}

// methods

std::string Rule::get_head_predicate() const {
    return head.get_predicate();
}

bool Rule::evaluate(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {}

//const std::unordered_map<std::string, std::vector<formula::Formula *>>
//Rule::get_body_variable_map() const {
//    return body.get_variable_map();
//};

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
    head.expire_outdated_groundings(current_time, current_tuple_counter);
    body.expire_outdated_groundings(current_time, current_tuple_counter);

}

formula::Formula &Rule::get_body_formula(size_t index) {
    return body.get_formula(index);
}

bool Rule::body_has_negated_predicates() const {
    return body.has_negated_predicates();
}

size_t Rule::get_body_size() const {
    return body.get_size();
}


} // namespace rule
} // namespace laser
