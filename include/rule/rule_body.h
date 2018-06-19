//
// Created by mike on 6/17/18.
//

#ifndef LASER_RULE_RULE_BODY_H
#define LASER_RULE_RULE_BODY_H

#include "formula/formula.h"
#include "formula/grounding_table.h"
#include <unordered_map>

namespace laser {
namespace rule {

class RuleBody {
private:
    int last_successful_join = -1;
    bool has_negated_atoms_m = false;

    /** All groundings that satisfy the body that are found when calling
     * the method RuleBody::holds_at() */
    formula::GroundingTable grounding_table = formula::GroundingTable();

    /* For the following Containers:
     * I am using vectors here rather than sets. It should not make sense for
     * the same formula to be present multiple times  in the same rule.
     */
    /** vector containing all formulas in the rule body */
    std::vector<formula::Formula *> formula_vector;
    /** vector containing all negated formulas in the rule body */
    std::vector<formula::Formula *> negated_formula_vector;
    /** positive_predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>> positive_predicate_map;
    /** negative_predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>> negative_predicate_map;
    /** predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>> predicate_map;
    /** predicate_map - key: predicate / value: pointer to Formula */
    std::unordered_map<std::string, std::vector<formula::Formula *>> variable_map;

// methods
    // void create_variable_map();

    /**
     * Called by constructor. Populates body datastructures:
     * negated_formula_vector, predicate_map, positive_predicate_map,
     * negative_predicate_map, variable_map.
     */
    void index_body_formulas();

    /**
     * Updates the grounding table of the body by joining with grounding table
     * of the formula and keeping the groundings that satisfy all  currently
     * evaluated formulas in the body.
     * @param formula
     * @param current_time
     */
    void evaluate_formula(formula::Formula const &formula,
                          long long int current_time);

    void do_math(formula::Formula const &left_formula,
                 formula::Formula const &right_formula,
                 int current_time);

    void
    do_comparison(formula::Formula const &left_formula,
                  formula::Formula const &right_formula,
                  int current_time);

    void
    accept_negated_substitution(formula::Formula *formula,
                                long long int current_time,
                                long long int current_tuple_counter);

    /**
     * Compiles a list containing all valid substitutions for he specified
     * variable in the groundings_table of the formula
     * @param variable
     * @param formula
     * @return Set of constants that are valid groundings for variable in
     * formula
     */
    std::set<std::string> const &
    get_variable_substitutions(std::string variable,
                               formula::Formula *formula) const;


public:

// constructors & destructors
    RuleBody(std::vector<formula::Formula *> formula_vector);

    ~RuleBody();

// getters & setters
    formula::GroundingTable const &get_grounding_table() const;

// const methods

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
    bool evaluate(long long int current_time,
                  long long int current_tuple_counter);

    /**
     * Removes all annotated grounding that have expired due to horizon time
     * or tupple counter
     */
    void expire_outdated_groundings(long long int current_time,
                                    long long int current_tuple_counter);

};

} // namespace rule
} // namespace laser


#endif // LASER_RULE_RULE_BODY_H
