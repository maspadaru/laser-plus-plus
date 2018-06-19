//
// Created by mike on 6/17/18.
//

#include "rule/rule_body.h"


namespace laser {
namespace rule {



// constructors & destructors


RuleBody::RuleBody(std::vector<formula::Formula *> formulas) :
        formula_vector(formulas) {}

RuleBody::~RuleBody() {
    formula_vector.clear();
}

// getters & setters
formula::GroundingTable const &RuleBody::get_grounding_table() const {}

// methods

void RuleBody::index_body_formulas() {}

void RuleBody::evaluate_formula(formula::Formula const &formula,
                                long long int current_time) {}

void RuleBody::do_math(formula::Formula const &left_formula,
                       formula::Formula const &right_formula,
                       int current_time) {}

void
RuleBody::do_comparison(formula::Formula const &left_formula,
                        formula::Formula const &right_formula,
                        int current_time) {}

void
RuleBody::accept_negated_substitution(formula::Formula *formula,
                                      long long int current_time,
                                      long long int current_tuple_counter) {}

std::set<std::string> const &
RuleBody::get_variable_substitutions(std::string variable,
                                     formula::Formula *formula) const {}


bool RuleBody::evaluate(long long int current_time,
                        long long int current_tuple_counter) {}

void RuleBody::expire_outdated_groundings(long long int current_time,
                                          long long int current_tuple_counter) {}


} // namespace rule
} // namespace laser