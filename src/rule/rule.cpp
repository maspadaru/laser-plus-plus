//
// Created by mike on 6/15/18.
//

#include "rule/rule.h"

#include <utility>

namespace laser {
namespace rule {

// constructors & destructors

Rule::Rule(formula::Formula *head_formula, formula::Formula *body_formula)
    : head(head_formula->clone()), body(body_formula->clone()) {
    compute_variable_map();
}

Rule::~Rule() {
    delete &head;
    delete &body;
}

Rule::Rule(Rule const &other)
    : head(other.head.clone()), body(other.body.clone()) {
    variable_map = other.variable_map;
}

Rule::Rule(Rule &&other) noexcept
    : head(other.head.move()), body(other.body.move()) {
    variable_map = std::move(other.variable_map);
}

Rule &Rule::operator=(Rule const &other) {
    this->head = other.head.clone();
    this->body = other.body.clone();
    this->variable_map = other.variable_map;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->head = other.head.move();
    this->body = other.body.move();
    variable_map = std::move(other.variable_map);
    return *this;
}

// getters & setters

formula::Formula &Rule::get_head() const {
    return this->head;
}

// methods

void Rule::expire_outdated_groundings(util::Timeline timeline) {
    head.expire_outdated_groundings(timeline);
    body.expire_outdated_groundings(timeline);
}

void Rule::debug_print() const {
    std::cerr << std::endl;
    std::cerr << "Rule -> head: "
        << head.debug_string();
    std::cerr << "Rule body:"
        << body.debug_string();
    std::cerr << "// Rule " << std::endl;
    std::cerr << std::endl;
}

bool Rule::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return body.evaluate(timeline, std::move(facts));
}

void Rule::compute_variable_map() {
    auto head_variable_vector = head.get_variable_names();
    for (size_t head_index = 0; head_index < head_variable_vector.size();
         head_index++) {
        auto body_index =
            body.get_variable_index(head_variable_vector.at(head_index));
        variable_map.insert({head_index, body_index});
    }
}

formula::Grounding
Rule::convert_to_head_grounding(formula::Grounding const &grounding, 
        util::Timeline timeline) {
    auto result = formula::Grounding(
        timeline.get_time(), timeline.get_time(),
        timeline.get_tuple_count(), timeline.get_tuple_count());
    std::vector<std::string> result_vector;
    for (size_t head_index = 0; head_index < head.get_number_of_variables();
         head_index++) {
        result_vector.push_back(
            grounding.get_substitution(variable_map.at(head_index)));
    }
    result.add_substitution_vector(result_vector);
    return result;
}

bool Rule::derive_conclusions(util::Timeline timeline) {
    if (body.is_satisfied()) {
        std::vector<formula::Grounding> predicate_facts;
        std::vector<formula::Grounding> body_groundings = body.get_groundings();
        for (auto const &grounding : body_groundings) {
            auto head_grounding = convert_to_head_grounding(grounding, timeline);
            predicate_facts.push_back(head_grounding);
        }
        std::unordered_map<std::string, std::vector<formula::Grounding>> body_facts;
        // a bit overcomplicated, but this will allow more flexibility in the head
        for (auto predicate : head.get_predicate_vector()) {
            body_facts.insert({predicate, predicate_facts});
        }
        auto result =
            head.evaluate(timeline, body_facts);
        return result;
    }
    return false;
}

template <typename T>
void Rule::debug_print(std::string const &message, T const &value) const {
    std::cerr << "Rule -> head predicate: "
              << this->head.get_predicate_vector().at(0) << " -> ";
    std::cerr << message << " : " << value << std::endl;
}

} // namespace rule
} // namespace laser
