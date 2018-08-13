#ifndef LASER_FORMULA_EXTENDED_NEGATION_H
#define LASER_FORMULA_EXTENDED_NEGATION_H

#include <string>

#include "formula/formula.h"
#include "formula/grounding.h"

namespace laser {
namespace formula {

/**
 * Time Window Formula
 */
class Negation : public Formula {
  private:
      Formula* child;

  public:
    // constructors / destructors

    ~Negation() override;

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

    bool
    evaluate(uint64_t current_time, uint64_t current_tuple_counter,
             std::unordered_map<std::string, std::vector<formula::Grounding>>
                 facts) override;

    size_t get_number_of_variables() const override;

    void expire_outdated_groundings(uint64_t current_time,
                                    uint64_t current_tuple_counter) override;

    std::vector<Grounding> get_groundings() const override;

    std::string debug_string() const override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_NEGATION_H
