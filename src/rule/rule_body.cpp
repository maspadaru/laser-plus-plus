//
// Created by mike on 6/17/18.
//

#include <utility>
#include <iostream>
#include "rule/rule_body.h"


namespace laser {
namespace rule {



// constructors & destructors

RuleBody::~RuleBody() {
    clear_rule_formulas();
}

RuleBody::RuleBody(std::vector<formula::Formula *> parameter_vector) {
    /* Because parameter_vector contains items that might have been allocated
     * on the stack, it's safest to clone all Formula* using formula.clone().*/
    copy_rule_formulas(std::move(parameter_vector));
    index_body_formulas();
}

RuleBody::RuleBody(RuleBody const &other) :
        last_successful_join(other.last_successful_join)

{
    /* Formula* need to be reallocated using formula.clone(), otherwise, they
     * might go out of scope along with &other. */
    copy_rule_formulas(other.formula_vector);
}

RuleBody::RuleBody(RuleBody &&other) noexcept :
        last_successful_join(other.last_successful_join),
        formula_vector(std::move(other.formula_vector)),
        negated_formula_vector(std::move(other.negated_formula_vector)),
        predicate_map(std::move(other.predicate_map)),
        positive_predicate_map(std::move(other.positive_predicate_map)),
        negated_predicate_map(std::move(other.negated_predicate_map)),
        variable_map(std::move(other.variable_map)) {}

RuleBody &RuleBody::operator=(RuleBody const &other) {
    clear_rule_formulas();
    copy_rule_formulas(other.formula_vector);
    this->last_successful_join = other.last_successful_join;
    index_body_formulas();
    return *this;
}

RuleBody &RuleBody::operator=(RuleBody &&other) noexcept {
    clear_rule_formulas();
    this->formula_vector = std::move(other.formula_vector);
    this->negated_formula_vector = std::move(other.negated_formula_vector);
    this->variable_map = std::move(other.variable_map);
    this->predicate_map = std::move(other.predicate_map);
    this->positive_predicate_map = std::move(other.positive_predicate_map);
    this->negated_predicate_map = std::move(other.negated_predicate_map);
    this->last_successful_join = other.last_successful_join;
}


// getters & setters

formula::GroundingTable RuleBody::get_grounding_table() const {}

const std::unordered_map<std::string, std::vector<formula::Formula *>>
RuleBody::get_variable_map() const {
    return variable_map;
}

std::unordered_map<std::string, std::vector<formula::Formula *>>
const &RuleBody::get_positive_predicate_map() const {
    return positive_predicate_map;
}

std::unordered_map<std::string, std::vector<formula::Formula *>>
const &RuleBody::get_negated_predicate_map() const {
    return negated_predicate_map;
}

// methods

void RuleBody::index_body_formulas() {
    negated_formula_vector.clear();
    variable_map.clear();
    predicate_map.clear();
    positive_predicate_map.clear();
    negated_predicate_map.clear();
    for (auto formula_pointer : formula_vector) {
        auto &formula = *formula_pointer;
        auto type = formula.get_type();
        if (type != formula::FormulaType::MATH &&
                type != formula::FormulaType::COMP) {
            auto predicate = formula.get_predicate();

            auto pos_neg_map = (formula.is_negated()) ? negated_predicate_map
                                                      : positive_predicate_map;
            pos_neg_map.try_emplace(predicate);
            std::vector<laser::formula::Formula *> &map_vector
                    = pos_neg_map[predicate];
            map_vector.push_back(&formula);

            predicate_map.try_emplace(predicate);
            map_vector = predicate_map[predicate];
            map_vector.push_back(&formula);

            for (auto const& variable_name :formula.get_variable_names()) {
                variable_map.try_emplace(variable_name);
                map_vector = variable_map[variable_name];
                map_vector.push_back(&formula);
            }
        }
    }
    is_indexed = true;
}

void
RuleBody::accept_negated_substitution(
        formula::Formula &formula,
        uint64_t current_time,
        uint64_t current_tuple_counter) {}

std::set<std::string>
RuleBody::get_variable_substitutions(
        std::string variable,
        formula::Formula &formula) const {}


bool RuleBody::evaluate(
        uint64_t current_time,
        uint64_t current_tuple_counter) {}

void RuleBody::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    for(auto formula :formula_vector) {
        formula->expire_outdated_groundings(current_time, current_tuple_counter);
    }
    body_grounding_table.expire_outdated_groundings(
            current_time, current_tuple_counter);
}

formula::GroundingTable
RuleBody::evaluate_formula(
        formula::GroundingTable grounding_table,
        formula::Formula const &formula,
        uint64_t current_time) {
    return formula::GroundingTable();
}

formula::GroundingTable
RuleBody::do_math(
        formula::GroundingTable grounding_table,
        formula::Formula const &formula, int current_time) {
    return formula::GroundingTable();
}

formula::GroundingTable
RuleBody::do_comparison(
        formula::GroundingTable grounding_table,
        formula::Formula const &formula, int current_time) {
    return formula::GroundingTable();
}

formula::Formula &RuleBody::get_formula(size_t index) const {
    return *formula_vector[index];
}

bool RuleBody::has_negated_predicates() const {
    return !(this->negated_predicate_map.empty());
}

size_t RuleBody::get_size() const {
    return formula_vector.size();
}

void RuleBody::clear_rule_formulas() {
    for (auto formula : formula_vector){
        delete formula;
    }
    formula_vector.clear();
}

void RuleBody::copy_rule_formulas(
        std::vector<formula::Formula *> other_vector) {
    for (auto formula : other_vector) {
        this->formula_vector.push_back(&formula->clone());
    }
    index_body_formulas();
}


} // namespace rule
} // namespace laser