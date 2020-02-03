#ifndef LASER_FORMULA_EXTENDED_CONJUNCTION_H
#define LASER_FORMULA_EXTENDED_CONJUNCTION_H

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "formula/formula.h"
#include "formula/grounding_table.h"
#include "util/grounding.h"

namespace laser::formula {

/**
 * CONJUNCTION Formula
 */
class Conjunction : public Formula {
  private:
    bool is_head_m = false;
    std::unique_ptr<formula::Formula> left_child;
    std::unique_ptr<formula::Formula> right_child;
    std::vector<std::string> predicate_vector;
    std::map<std::string, size_t> arity_map;
    std::vector<std::string> common_child_variables;
    std::vector<std::string> variable_names;
    std::vector<int> left_variable_indices;
    std::vector<int> right_variable_indices;
    size_t number_of_variables = 0;
    std::vector<int> left_common_variable_indices;
    std::vector<int> right_common_variable_indices;
    size_t number_of_common_variables = 0;

    /**
     * Variable Name -> position in variable_vector and groundings.
     */
    std::unordered_map<std::string, size_t> variable_map;

    // std::unordered_set<std::shared_ptr<util::Grounding>,
    // util::GroundingFullHash, util::GroundingEqual>
    // grounding_set;
    std::set<std::shared_ptr<util::Grounding>, util::GroundingFullCompare>
        grounding_set;
     //std::vector<std::shared_ptr<util::Grounding>>
     //grounding_vector;

    void compute_predicate_vector();

    void compute_arity_map();

    void set_head(bool is_head) override;

    bool is_head() const override;

    void concatenate_vectors(std::vector<std::string> const &left,
                             std::vector<std::string> const &right);

    void populate_variable_collections();

    void populate_grounding_set(
        util::Timeline const &timeline, size_t previous_step);

    void populate_grounding_set_with_common(
        util::Timeline const &timeline, size_t previous_step);

    void populate_grounding_set_no_common(
        util::Timeline const &timeline, size_t previous_step);

    std::string
    hash_common_variables(std::vector<int> const &common_variable_indices,
                          util::Grounding const &grounding) const;

    std::shared_ptr<util::Grounding>
    merge_groundings(util::Timeline const &timeline,
                     util::Grounding const &left,
                     util::Grounding const &right) const;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) const;

  public:
    Conjunction() = default;
    explicit Conjunction(std::unique_ptr<formula::Formula> left_child,
                         std::unique_ptr<formula::Formula> right_child);
    explicit Conjunction(std::unique_ptr<formula::Formula> left_child,
                         std::unique_ptr<formula::Formula> right_child,
                         bool is_head);
    ~Conjunction() = default;

    std::unique_ptr<formula::Formula> clone() const override;

    FormulaType get_type() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;


    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_new_facts(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_old_facts(util::Timeline const &timeline) override;

    void new_step(uint64_t current_time) override; 

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    void add_child(std::unique_ptr<formula::Formula> child) override;

    std::vector<std::unique_ptr<formula::Formula> const *>
    get_children() const override;

    uint64_t get_window_size() const override;
};

} // namespace laser::formula

#endif // LASER_FORMULA_EXTENDED_CONJUNCTION_H
