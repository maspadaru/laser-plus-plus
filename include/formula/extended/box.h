#ifndef LASER_FORMULA_EXTENDED_BOX_H
#define LASER_FORMULA_EXTENDED_BOX_H

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "util/grounding.h"

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

    std::unordered_map<size_t, std::shared_ptr<util::Grounding>> box_map;

    // Methods

    std::pair<bool, std::shared_ptr<util::Grounding>>
    adjust_annotation(std::shared_ptr<util::Grounding> const &box_grounding,
                      std::shared_ptr<util::Grounding> const &child_grounding,
                      util::Timeline const &timeline) const;

    std::vector<std::shared_ptr<util::Grounding>>
    compute_box_conclusions(util::Timeline const &timeline);

    void
    update_box_map(std::vector<std::shared_ptr<util::Grounding>> const &facts,
                   util::Timeline const &timeline);

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

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    bool evaluate(
        util::Timeline const &timeline, util::Database const &database,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_BOX_H
