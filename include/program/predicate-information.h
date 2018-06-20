//
// Created by mike on 6/20/18.
//

#ifndef LASER_PROGRAM_PREDICATE_INFORMATION_H
#define LASER_PROGRAM_PREDICATE_INFORMATION_H

#include "rule/rule.h"
#include "formula/formula.h"

namespace laser {
namespace program {

/**
 *
 */
class PredicateInformation {
private:

    std::string predicate;
    bool is_head_of_rule_m;
    bool is_negated_m;

    /* rule having this predicate as head
     * nullptr if !is_head_of_rule_m
     * TODO: can multiple rules in the same stratum have this pred as head? */
    rule::Rule *rule;

    /* All formulas in this stratum having this predicate */
    std::vector<formula::Formula *> formula_vector;


// methods

public:

// constructors & destructors

    PredicateInformation(const std::string predicate, bool is_head_of_rule_m,
                         bool is_negated_m, rule::Rule *rule,
                         std::vector<formula::Formula *> formula_vector);

    PredicateInformation(const std::string predicate, bool is_head_of_rule,
                         bool is_negated, rule::Rule *rule,
                         formula::Formula *formula);

    ~PredicateInformation() = default;

// getters & setters

    const std::string get_predicate() const;

    bool is_head_of_rule() const;

    bool is_negated() const;

    rule::Rule *get_rule() const;

    const std::vector<formula::Formula *> get_formula_vector() const;

// methods

    void add_formula(formula::Formula *formula);

};

} // namespace program
} // namespace laser


#endif // LASER_PROGRAM_PREDICATE_INFORMATION_H
