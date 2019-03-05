#ifndef LASER_FORMULA_EXTENDED_DIAMOND_H
#define LASER_FORMULA_EXTENDED_DIAMOND_H

#include <string>
#include <memory>

#include "formula/formula.h"
#include "formula/grounding.h"
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

/**
 * Dimond Formula
 */
class Diamond : public Formula {
  private:
    bool is_head_m = false;
    Formula *child;
    GroundingTable grounding_table;

  public:
    // constructors / destructors

    Diamond() = default;
    explicit Diamond(Formula* child);
    ~Diamond() override;

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

    std::vector<std::shared_ptr<Grounding>> get_groundings(util::Timeline const &timeline) override;
    
    std::vector<std::shared_ptr<Grounding>> get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<Grounding>> get_conclusions_step(util::Timeline const &timeline) override;

    bool
    evaluate(util::Timeline const &timeline,
             std::unordered_map<std::string, std::vector<std::shared_ptr<Grounding>>>
                 const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;
    
    void add_child(formula::Formula* child) override;

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_DIAMOND_H
