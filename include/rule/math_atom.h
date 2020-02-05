#ifndef LASER_RULE_MATH_ATOM_H
#define LASER_RULE_MATH_ATOM_H

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "util/grounding.h"
#include "formula/formula.h"

namespace laser::rule {

/**
 * Math formula
 * result = left + right
 * Requirement: The result variable should not be used in other places in the
 * rule. Use Comparison Atom for that (not yet implemented).
 */
class MathAtom {
  private:
    std::string result_name;
    std::string left_name;
    int64_t right_value;

  public:
    MathAtom() = default;

    ~MathAtom() = default;

    void set_left_name();
    void set_result_name();
    void set_right_value();
    std::string const &get_result_name() const;

    int64_t get_result_value();

    void evaluate(std::unique_ptr<formula::Formula> const &body,
                  std::vector<std::shared_ptr<util::Grounding>> &body_facts);
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_ATOM_H
