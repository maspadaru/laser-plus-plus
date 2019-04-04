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
    init();
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

formula::Formula &Rule::get_head() const { return this->head; }

// methods

void Rule::expire_outdated_groundings(util::Timeline const &timeline) {
    head.expire_outdated_groundings(timeline);
    body.expire_outdated_groundings(timeline);
}

void Rule::evaluate(
    util::Timeline const &timeline,
    util::Database const &database) {
    auto facts = database.get_data_since(previous_step);
    body.evaluate(timeline, facts);
}

void Rule::init() {
    head.set_head(true);
    compute_variable_map();
}

void Rule::compute_variable_map() {
    auto head_variable_vector = head.get_variable_names();
    for (size_t head_index = 0; head_index < head_variable_vector.size();
         head_index++) {
        auto body_index =
            body.get_variable_index(head_variable_vector.at(head_index));
        variable_map.try_emplace(head_index, body_index);
    }
}

std::shared_ptr<util::Grounding>
Rule::convert_to_head_grounding(std::string const &head_predicate,
                                util::Grounding const &grounding) const {
    std::vector<std::string> result_vector;
    for (size_t head_index = 0; head_index < head.get_number_of_variables();
         head_index++) {
        result_vector.push_back(
            grounding.get_constant(variable_map.at(head_index)));
    }
    util::Grounding result = util::Grounding(
        head_predicate, grounding.get_consideration_time(),
        grounding.get_horizon_time(), grounding.get_consideration_count(),
        grounding.get_horizon_count(), result_vector);
    return std::make_shared<util::Grounding>(result);
}

bool Rule::derive_conclusions(util::Timeline const &timeline) {
    bool result = false;
    auto head_predicate = head.get_predicate_vector().at(0);
    std::vector<std::shared_ptr<util::Grounding>> head_facts;
    std::vector<std::shared_ptr<util::Grounding>> body_groundings =
        body.get_groundings(timeline);
    for (auto const &body_grounding : body_groundings) {
        // SNE: we only evaluate groundings derived at this current timepoint
        if (body_grounding->get_consideration_time() >= timeline.get_time()) {
            auto head_grounding =
                convert_to_head_grounding(head_predicate, *body_grounding);
            head_facts.push_back(std::move(head_grounding));
        }
    }
    bool is_body_satisfied = !head_facts.empty();
    if (is_body_satisfied) {
        result = head.evaluate(timeline, head_facts);
    }
    return result;
}


bool Rule::is_existential() const {
    return head.get_type() == formula::FormulaType::EXISTENTIAL;
}

void Rule::reset_previous_step() {
    previous_step = 0;
}

void Rule::set_previous_step(size_t step) {
    previous_step = step;
}

} // namespace rule
} // namespace laser
