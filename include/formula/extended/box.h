#ifndef LASER_FORMULA_EXTENDED_BOX_H
#define LASER_FORMULA_EXTENDED_BOX_H

#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <memory>

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

    std::unordered_map<size_t, std::shared_ptr<Grounding>> box_map;

    // Methods

    std::pair<bool, std::shared_ptr<Grounding>>
    adjust_annotation(std::shared_ptr<Grounding> const &box_grounding,
                      std::shared_ptr<Grounding> const &child_grounding,
                      util::Timeline timeline) const;

    std::vector<std::shared_ptr<Grounding>> compute_box_conclusions(util::Timeline timeline);

    void update_box_map(std::vector<std::shared_ptr<Grounding>> const &facts,
            util::Timeline timeline);

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

    std::vector<std::shared_ptr<Grounding>> get_groundings(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_timepoint(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_step(util::Timeline timeline) override;

    std::string debug_string() const override;

    bool evaluate(
        util::Timeline timeline,
        std::unordered_map<std::string, std::vector<std::shared_ptr<Grounding>>> const
            &facts) override;

    void expire_outdated_groundings(util::Timeline timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_BOX_H
