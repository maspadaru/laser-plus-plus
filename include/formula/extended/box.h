#ifndef LASER_FORMULA_EXTENDED_BOX_H
#define LASER_FORMULA_EXTENDED_BOX_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "formula/formula.h"
#include "formula/grounding.h"
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

/**
 * Box Formula
 */
class Box : public Formula {
  private:
    bool is_head_m = false;

    Formula *child;

    GroundingTable grounding_table;

    std::unordered_map<std::string, Grounding> box_map;

    // Methods

    bool adjust_annotation(Grounding &box_grounding,
                           Grounding const &child_grounding);

    std::vector<Grounding>
    compute_box_conclusions(util::Timeline timeline) const;

    void update_box_map(std::vector<Grounding> const &facts);

  public:
    // constructors / destructors

    Box() = default;
    explicit Box(Formula *child);
    ~Box() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    // getters / setters

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    // methods

    std::vector<std::string> get_variable_names() const override;

    std::vector<std::string> get_full_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    size_t get_number_of_variables() const override;

    std::vector<Grounding>
    get_groundings(util::Timeline timeline) override;

    std::vector<Grounding> get_conclusions_timepoint(util::Timeline timeline) override;

    std::vector<Grounding> get_conclusions_step(util::Timeline timeline) override;

    std::string debug_string() const override;

    bool
    evaluate(util::Timeline timeline,
             std::unordered_map<std::string, std::vector<formula::Grounding>>
                 facts) override;

    void expire_outdated_groundings(util::Timeline timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_BOX_H
