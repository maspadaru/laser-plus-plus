#ifndef LASER_FORMULA_EXTENDED_DIAMOND_H
#define LASER_FORMULA_EXTENDED_DIAMOND_H

#include <string>

#include "formula/formula.h"
#include "formula/grounding.h"

namespace laser {
namespace formula {

/**
 * Time Window Formula
 */
class Diamond : public Formula {
  private:
    Formula *child;

  public:
    // constructors / destructors

    Diamond() = default;
    ~Diamond() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    // getters / setters

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    // methods

    std::vector<std::string> get_variable_names() const override;

    std::vector<std::string> get_full_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    size_t get_number_of_variables() const override;

    std::vector<Grounding> get_groundings() const override;

    std::string debug_string() const override;
    
    bool
    evaluate(util::Timeline timeline,
             std::unordered_map<std::string, std::vector<formula::Grounding>>
                 facts) override;

    void expire_outdated_groundings(util::Timeline timeline) override;
    
    void add_child(formula::Formula* child) override;

    explicit Diamond(Formula* child);
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_DIAMOND_H
