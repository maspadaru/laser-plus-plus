#ifndef LASER_RULE_MATH_ATOM_H
#define LASER_RULE_MATH_ATOM_H

#include <memory>
#include <string>
#include <vector>
#include <cstdlib>

#include "util/grounding.h"

namespace laser::rule {

/**
 * Math formula
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

    std::vector<std::shared_ptr<util::Grounding>>
    evaluate(std::vector<std::shared_ptr<util::Grounding>> body_facts);
};

} // namespace laser::rule

#endif // LASER_RULE_MATH_ATOM_H
