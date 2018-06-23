//
// Created by mike on 6/17/18.
//

#include <utility>
#include "rule/rule_body.h"


namespace laser {
namespace rule {



// constructors & destructors


//RuleBody::RuleBody(std::vector<formula::Formula &> formulas) :
//        formula_vector(std::move(formulas)) {
//    index_body_formulas();
//}



RuleBody::~RuleBody() {
    formula_vector.clear();
}

RuleBody::RuleBody(RuleBody const &other) {
    this->last_successful_join = other.last_successful_join;
    for (auto formula : other.formula_vector) {
        this->formula_vector.push_back(&formula->clone());
    }
    index_body_formulas();
}

RuleBody::RuleBody(RuleBody &&other) noexcept {
    this->formula_vector = std::move(other.formula_vector);

    this->negated_formula_vector = std::move(other.negated_formula_vector);
    this->variable_map = std::move(other.variable_map);
    this->predicate_map = std::move(other.predicate_map);
    this->positive_predicate_map = std::move(other.positive_predicate_map);
    this->negated_predicate_map = std::move(other.negated_predicate_map);

    this->last_successful_join = other.last_successful_join;
}

RuleBody &RuleBody::operator=(RuleBody const &other) {
    this->last_successful_join = other.last_successful_join;

    this->formula_vector.clear();
    for (auto formula : other.formula_vector) {
        this->formula_vector.push_back(&formula->clone());
    }
    index_body_formulas();

    return *this;
}

RuleBody &RuleBody::operator=(RuleBody &&other) noexcept {
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

            for (auto variable_name :formula.get_variable_names()) {
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
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {}

std::set<std::string>
RuleBody::get_variable_substitutions(
        std::string variable,
        formula::Formula &formula) const {}


bool RuleBody::evaluate(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {}

void RuleBody::expire_outdated_groundings(
        unsigned long long int current_time,
        unsigned long long int current_tuple_counter) {
    // TODO
}

formula::GroundingTable
RuleBody::evaluate_formula(
        formula::GroundingTable grounding_table,
        formula::Formula const &formula,
        unsigned long long int current_time) {
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

void RuleBody::add_formula(formula::Formula &formula) {
    formula::Formula &to_add = formula.move();
    this->formula_vector.push_back(&to_add);
    is_indexed = false;
}

formula::Formula& RuleBody::get_formula(size_t index) const {
    return *formula_vector[index];
}

bool RuleBody::has_negated_predicates() const {
    return !(this->negated_predicate_map.empty());
}


} // namespace rule
} // namespace laser