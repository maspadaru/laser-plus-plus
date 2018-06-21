//
// Created by mike on 6/17/18.
//

#include <utility>
#include "rule/rule_body.h"


namespace laser {
namespace rule {



// constructors & destructors


RuleBody::RuleBody(std::vector<formula::Formula *> formulas) :
        formula_vector(std::move(formulas)) {}

RuleBody::~RuleBody() {
    formula_vector.clear();
}

// getters & setters

formula::GroundingTable RuleBody::get_grounding_table() const {}

const std::unordered_map<std::string, std::vector<formula::Formula *>>
RuleBody::get_variable_map() const {
    return variable_map;
}

const std::unordered_map<std::string, std::vector<formula::Formula *>>
RuleBody::get_positive_predicate_map() const {
    return positive_predicate_map;
}

const std::unordered_map<std::string, std::vector<formula::Formula *>>
RuleBody::get_negated_predicate_map() const {
    return negated_predicate_map;
}

// methods

void RuleBody::index_body_formulas() {
    for (auto formula : formula_vector) {
        auto type = formula->get_type();
        if (type != formula::FormulaType::MATH &&
            type != formula::FormulaType::COMP) {
            auto predicate = formula->get_predicate();

            auto pos_neg_map = (formula->is_negated()) ? negated_predicate_map
                                                       : positive_predicate_map;
            pos_neg_map.try_emplace(predicate);
            std::vector<laser::formula::Formula *> &map_vector
                    = pos_neg_map[predicate];
            map_vector.push_back(formula);

            predicate_map.try_emplace(predicate);
            map_vector = predicate_map[predicate];
            map_vector.push_back(formula);

            for (auto variable_name :formula->get_variable_names()) {
                variable_map.try_emplace(variable_name);
                map_vector = variable_map[variable_name];
                map_vector.push_back(formula);


            }

        }

    }
}

void
RuleBody::accept_negated_substitution(formula::Formula *formula,
                                      unsigned long long int current_time,
                                      unsigned long long int current_tuple_counter) {}

std::set<std::string>
RuleBody::get_variable_substitutions(std::string variable,
                                     formula::Formula *formula) const {}


bool RuleBody::evaluate(unsigned long long int current_time,
                        unsigned long long int current_tuple_counter) {}

void RuleBody::expire_outdated_groundings(unsigned long long int current_time,
                                          unsigned long long int current_tuple_counter) {
    // TODO
}

formula::GroundingTable
RuleBody::evaluate_formula(formula::GroundingTable grounding_table,
                           formula::Formula const &formula,
                           unsigned long long int current_time) {
    return formula::GroundingTable();
}

formula::GroundingTable
RuleBody::do_math(formula::GroundingTable grounding_table,
                  formula::Formula const &formula, int current_time) {
    return formula::GroundingTable();
}

formula::GroundingTable
RuleBody::do_comparison(formula::GroundingTable grounding_table,
                        formula::Formula const &formula, int current_time) {
    return formula::GroundingTable();
}


} // namespace rule
} // namespace laser