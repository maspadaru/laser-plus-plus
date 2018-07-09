//
// Created by mike on 6/17/18.
//

#include <utility>
#include "program/stratum.h"

namespace laser {
namespace program {

// constructors & destructors

// getters & setters

// methods


void
Stratum::add_head_predicate(
        std::string predicate, formula::Formula &rule_head,
        rule::Rule *rule, std::vector<rule::Rule *> rule_vector) {
    PredicateInformation pi = PredicateInformation(
            std::move(predicate), true, false,
            rule, rule_head, std::move(rule_vector));
    predicate_vector.push_back(pi);

}

void Stratum::add_nonhead_predicate(
        std::string predicate, bool is_negated,
        std::vector<formula::Formula *> formula_vector,
        std::vector<rule::Rule *> rule_vector) {

    PredicateInformation pi = PredicateInformation(
            std::move(predicate), false,
            is_negated,
            nullptr,
            std::move(formula_vector), std::move(rule_vector));
    predicate_vector.push_back(pi);
}

size_t Stratum::size() {
    return predicate_vector.size();
}

void Stratum::deduplicate() {
    //TODO
}


void Stratum::evaluate_non_head(
        PredicateInformation predicate_information, uint64_t time_point,
        uint64_t tuple_counter,
        std::vector<formula::Formula *> new_facts) {
    auto formula_vector = predicate_information.get_formula_vector();
    for (auto formula : formula_vector) {
        if(predicate_information.is_negated()) {
            // TODO	node.alreadyhadInput= True
        } else {
            formula->accept(time_point, tuple_counter, new_facts);
        }
    }
}

void Stratum::evaluate_head_of_other_rule(
        PredicateInformation predicate_information, uint64_t time_point,
        uint64_t tuple_counter) {
    auto formula_vector = predicate_information.get_formula_vector();
    auto rule_vector = predicate_information.get_rule_vector();
    for (auto formula : formula_vector) {
        for (auto rule : rule_vector) {
            formula::Formula &head = rule->get_head();
            if(predicate_information.is_negated()) {
                // TODO if head.substitutetable.size() != 0:
                // TODO node.alreadyhadInput = True
            } else {
                formula->accept(time_point, tuple_counter,
                        head.get_recent_groundings());
            }
        }
    }
}

bool Stratum::evaluate(
        uint64_t time_point, uint64_t tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        new_facts) {
    bool result = false;
    for (auto const& predicate_information : predicate_vector) {
        if (predicate_information.is_head_of_rule()) {
            bool has_new_derivations = predicate_information.get_rule()
                    ->evaluate(time_point, tuple_counter);
            result = result || has_new_derivations;
        } else {
            auto predicate = predicate_information.get_predicate();
            auto found_new_facts = new_facts.find(predicate);
            if (found_new_facts != new_facts.end()) {
                // new facts containing this predicate are in the stream
                auto predicate_facts = found_new_facts->second;
                evaluate_non_head(predicate_information,
                        time_point, tuple_counter,
                        predicate_facts);
            } else {
                // no new facts containing this predicate are in the stream
                auto const &rule_vector
                        = predicate_information.get_rule_vector();
                if (!rule_vector.empty()) {
                    // this predicate occurs in some other rule's head
                    evaluate_head_of_other_rule(predicate_information,
                            time_point, tuple_counter);
                } else if (predicate_information.is_negated()) {
                    // TODO seen_positive_instance = false
                }
            }
        }
    }
}


} // namespace program
} // namespace laser
