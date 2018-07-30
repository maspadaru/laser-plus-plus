//
// Created by mike on 6/20/18.
//

#include <algorithm>
#include <excetion/uninitialized_exception.h>
#include "program/strata.h"

namespace laser {
namespace program {

// constructors & destructors

// getters & setters

// methods

void Strata::map_head_predicate_to_rule(
        std::vector<rule::Rule *> const &rule_vector) {
    for (auto rule : rule_vector) {
        auto head_predicate = rule->get_head().get_predicate();
        head_predicate_map.try_emplace(head_predicate);
        auto &map_value = head_predicate_map[head_predicate];
        map_value.push_back(rule);
    }
}

void Strata::stratify(std::vector<rule::Rule *> rule_vector) {
    map_head_predicate_to_rule(rule_vector);
    std::vector<Stratum> temp_stratum_vector;
    size_t stratum_index = 0;
    temp_stratum_vector.emplace_back();
    for (auto rule : rule_vector) {
        check_rule_dependencies(stratum_index, rule,
                &temp_stratum_vector);
        auto &head = rule->get_head();
        auto predicate = head.get_predicate();
        head_predicate_map.try_emplace(predicate);
        auto &head_predicate_vector = head_predicate_map[predicate];
        temp_stratum_vector[stratum_index].add_head_predicate(predicate, head,
                rule, head_predicate_vector);
    }
    std::copy_if(temp_stratum_vector.begin(),
            temp_stratum_vector.end(), std::back_inserter(stratum_vector),
            [](Stratum stratum) { return stratum.size() > 0; });
    std::reverse(stratum_vector.begin(), stratum_vector.end());
    deduplicate(0);
    is_stratified = true;
}

void Strata::check_rule_dependencies(
        size_t stratum_index, rule::Rule *rule,
        std::vector<Stratum> *temp_stratum_vector) {
    bool is_negated = false;
    follow_rule_dependencies(stratum_index, is_negated, rule,
            temp_stratum_vector);
    if (rule->body_has_negated_predicates()) {
        is_negated = true;
        stratum_index++;
        temp_stratum_vector->emplace_back();
        follow_rule_dependencies(stratum_index, is_negated, rule,
                temp_stratum_vector);
    }
}

void Strata::follow_rule_dependencies(
        size_t stratum_index,
        bool is_negated,
        rule::Rule *rule,
        std::vector<laser::program::Stratum> *temp_stratum_vector) {
    auto predicate_map = is_negated ?
                         rule->get_body_negated_predicate_map() :
                         rule->get_body_positive_predicate_map();
    for (const auto &iterator : predicate_map) {
        std::string predicate = iterator.first;
        std::vector<formula::Formula *> &formula_vector =
                predicate_map[predicate];

        head_predicate_map.try_emplace(predicate);
        auto &head_predicate_vector = head_predicate_map[predicate];
        // if predicate is the head of another rule, head_predicate_vector
        // will not be empty
        for (rule::Rule
                    *head_predicate_rule : head_predicate_vector) {
            check_rule_dependencies(stratum_index,
                    head_predicate_rule,
                    temp_stratum_vector);
            Stratum &stratum = temp_stratum_vector->at(stratum_index);
            stratum.add_head_predicate(predicate,
                    head_predicate_rule->get_head(),
                    head_predicate_rule,
                    head_predicate_vector);
        }
        Stratum &stratum = temp_stratum_vector->at(stratum_index);
        stratum.add_nonhead_predicate(predicate, is_negated, formula_vector,
                head_predicate_vector);
    }
}


void Strata::deduplicate(size_t stratum_index) {
    // TODO deduplicate_stratas(self, strataList, idx) in python
    // TODO should call stratum.deduplicate instead of deDup(self, strata)
}

bool Strata::evaluate(
        uint64_t time_point, uint64_t tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        new_facts) {
    if (!is_stratified) {
        throw exception::UninitializedException("stratify() needs to be called"
                                                "before evaluate() in order"
                                                "to initialize the strata");
    }
    bool result = false;
    for (auto stratum :stratum_vector) {
        result = result ||
                stratum.evaluate(time_point, tuple_counter, new_facts);
    }
    return result;
}

} // namespace program
} // namespace laser

