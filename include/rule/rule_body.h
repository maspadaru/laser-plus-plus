//
// Created by mike on 6/17/18.
//

#ifndef LASER_RULE_RULE_BODY_H
#define LASER_RULE_RULE_BODY_H

#include <unordered_map>

#include "formula/formula.h"
#include "formula/grounding_table.h"

namespace laser {
namespace rule {

class RuleBody {
private:
    int last_successful_join = -1;
    bool is_indexed = false;

    /* All groundings that satisfy the body that are found when calling
     * the method RuleBody::holds_at() */
    formula::GroundingTable body_grounding_table = formula::GroundingTable();

    /* For the following Containers:
     * I am using vectors here rather than sets. It should not make sense for
     * the same formula to be present multiple times  in the same rule.
     */

    /* vector containing all formulas in the rule body */
    std::vector<formula::Formula *> formula_vector;

    /* vector containing all negated formulas in the rule body */
    std::vector<formula::Formula *> negated_formula_vector;

    /* positive_predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            positive_predicate_map;

    /* negated_predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            negated_predicate_map;

    /* predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            predicate_map;

    /* variable_map - key: variable name /
     * value: pointer to formula containing the variable */
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            variable_map;

// methods

    /**
     * Called by constructor. Populates body datastructures:
     * negated_formula_vector, predicate_map, positive_predicate_map,
     * negated_predicate_map, variable_map.
     */
    void index_body_formulas();

    // TODO this was the join function from python:
    /**
     * Computes the grounding table by joining with the grounding table
     * of the formula and keeping the groundings that satisfy both.
     * @param formula
     * @param current_time
     */
    formula::GroundingTable evaluate_formula(
            formula::GroundingTable grounding_table,
            formula::Formula const &formula,
            uint64_t current_time);

    formula::GroundingTable do_math(
            formula::GroundingTable grounding_table,
            formula::Formula const &formula,
            int current_time);

    formula::GroundingTable
    do_comparison(
            formula::GroundingTable grounding_table,
            formula::Formula const &formula,
            int current_time);

    void
    accept_negated_substitution(
            formula::Formula &formula,
            uint64_t current_time,
            uint64_t current_tuple_counter);

    /**
     * Compiles a list containing all valid substitutions for he specified
     * variable in the groundings_table of the formula
     * @param variable
     * @param formula
     * @return Set of constants that are valid groundings for variable in
     * formula
     */
    std::set<std::string>
    get_variable_substitutions(
            std::string variable,
            formula::Formula &formula) const;

    //DELETE THESE:
    const std::unordered_map<std::string, std::vector<formula::Formula *>>
    get_variable_map() const;

    formula::GroundingTable get_grounding_table() const;

    void clear_rule_formulas();

    void copy_rule_formulas(std::vector<formula::Formula *> other_vector);

public:

// constructors & destructors
    RuleBody(RuleBody const &other); // copy constructor

    RuleBody(RuleBody &&other) noexcept ; // move constructor

    explicit RuleBody(std::vector<formula::Formula*> parameter_vector);

    ~RuleBody();

    RuleBody &operator=(RuleBody const &other);  // copy assignment

    RuleBody &operator=(RuleBody &&other) noexcept;  // move assignment

// getters & setters



// methods

    /**
     * Verifies if at least one grounding can be found that satisfies all
     * formulas in the the body.
     * All satisfying groundings that are found in the
     * process are added to a grounding table and can be retrieved by calling:
     * get_grounding_table();
     *
     * @param facts Vector of all intentional atoms from the data stream at the
     * current time-point
     * @param current_time
     * @param current_tuple_counter
     * @return True if at least one grounding that satisfies the body is found
     */
    bool evaluate(
            uint64_t current_time,
            uint64_t current_tuple_counter);

    /**
     * Removes all annotated grounding that have expired due to horizon time
     * or tuple counter
     */
    void expire_outdated_groundings(
            uint64_t current_time,
            uint64_t current_tuple_counter);

    formula::Formula& get_formula(size_t index) const;

    bool has_negated_predicates() const;

    std::unordered_map<std::string, std::vector<formula::Formula *>>
    const& get_positive_predicate_map() const;

    std::unordered_map<std::string, std::vector<formula::Formula *>>
    const& get_negated_predicate_map() const;

    size_t get_size() const;



};


} // namespace rule
} // namespace laser


#endif // LASER_RULE_RULE_BODY_H
