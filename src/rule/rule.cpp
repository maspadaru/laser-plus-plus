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
        std::vector<formula::Formula *> body_vector) :
        head(head_formula->clone()),
        body(std::move(body_vector)) {}

Rule::~Rule() {
    delete &head;
}

Rule::Rule(Rule const &other) : head(other.head.clone()), body(other.body) {}

Rule::Rule(Rule &&other) noexcept  : head(other.head.move()),
        body(std::move(other.body)) {
}

Rule &Rule::operator=(Rule const &other) {
    this->head = other.head.clone();
    this->body = RuleBody(other.body);
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->head = other.head.move();
    this->body = std::move(other.body);
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
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    // TODO implement
    // if body.evaluate() == true:
    //      derive new conclusions from the head.
    //      Return true if new conclusions were derived.
    return false;
}

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
        uint64_t current_time,
        uint64_t current_tuple_counter) {
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

void Rule::debug_print() const {
    std::cerr << std::endl;
    std::cerr << "Rule -> head: ";
    head.debug_print();
    std::cerr <<  "Rule body size = "  << body.get_size() ;

    std::cerr << "; Body predicates: " << std::endl;
    for(size_t i = 0; i < body.get_size(); i++) {
        auto &formula = body.get_formula(i);
        formula.debug_print();
    }
    std::cerr << "// Rule " << std::endl;
    std::cerr << std::endl;

}


} // namespace rule
} // namespace laser
