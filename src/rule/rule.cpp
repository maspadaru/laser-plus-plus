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
        formula::Formula *body_formula) :
        head(head_formula->clone()),
        body(body_formula->clone()) {}

Rule::~Rule() {
    delete &head;
    delete &body;
}

Rule::Rule(Rule const &other) : head(other.head.clone()), body(other.body) {}

Rule::Rule(Rule &&other) noexcept  : head(other.head.move()),
        body(other.body.move()) {
}

Rule &Rule::operator=(Rule const &other) {
    this->head = other.head.clone();
    this->body = other.body.clone();
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->head = other.head.move();
    this->body = std::move(other.body);
    return *this;
}

// getters & setters
// methods

void Rule::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    head.expire_outdated_groundings(current_time, current_tuple_counter);
    body.expire_outdated_groundings(current_time, current_tuple_counter);

}

void Rule::debug_print() const {
    std::cerr << std::endl;
    std::cerr << "Rule -> head: ";
    head.debug_print();
    std::cerr <<  "Rule body:" ;
    body.debug_print();
    std::cerr << "// Rule " << std::endl;
    std::cerr << std::endl;

}

bool Rule::evaluate(
        uint64_t current_time, uint64_t current_tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>> facts) {
    return body.evaluate(current_time, current_tuple_counter, facts);
}


} // namespace rule
} // namespace laser
