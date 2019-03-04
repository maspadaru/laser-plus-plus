#ifndef LASER_FORMULA_EXTENDED_CONJUNCTION_H
#define LASER_FORMULA_EXTENDED_CONJUNCTION_H

#include <string>
#include <unordered_map>
#include <vector>

#include "formula/formula.h"
#include "formula/grounding.h"
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

/**
 * CONJUNCTION Formula
 */
class Conjunction : public Formula {
  private:
    bool is_head_m = false;
    Formula *left_child;
    Formula *right_child;

    std::vector<std::string> common_child_variables;

    std::vector<std::string> variable_names;

    /**
     * Variable Name -> position in variable_vector and groundings.
     */
    std::unordered_map<std::string, size_t> variable_map;

    std::vector<std::shared_ptr<Grounding>> grounding_vector;

    // Methods

    void set_head(bool is_head) override;

    bool is_head() const override;

    template <typename T, typename A>
    std::vector<T, A> concatenate_vectors(std::vector<T, A> left,
                                          std::vector<T, A> right) const;

    void populate_variable_collections();
    void populate_grounding_vector(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<Grounding>> const &left_groundings,
        std::vector<std::shared_ptr<Grounding>> const &right_groundings);

    std::string hash_common_variables(Formula const &child,
                                      Grounding const &grounding) const;

    std::shared_ptr<Grounding> merge_groundings(util::Timeline const &timeline,
                                                Grounding const &left,
                                                Grounding const &right) const;

  public:
    // constructors / destructors

    Conjunction() = default;
    explicit Conjunction(Formula *left_child, Formula *right_child);
    ~Conjunction() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    // getters / setters

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    // methods

    std::vector<std::string> const &get_variable_names() const override;

    std::vector<std::string> const &get_full_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    bool is_satisfied() const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::string debug_string() const override;

    bool
    evaluate(util::Timeline const &timeline,
             std::unordered_map<std::string,
                                std::vector<std::shared_ptr<Grounding>>> const
                 &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_CONJUNCTION_H
