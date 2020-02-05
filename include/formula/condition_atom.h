#ifndef LASER_FORMULA_CONDITION_ATOM_H
#define LASER_FORMULA_CONDITION_ATOM_H

#include <memory>
#include <string>
#include <vector>

#include "formula/formula.h"
#include "util/grounding.h"

namespace laser::formula {

class ConditionAtom {
  private:
    std::string result_name;
    std::string left_name;
    int64_t right_value;

  public:
    ConditionAtom() = default;

    ~ConditionAtom() = default;

    void set_left_name();
    void set_result_name();
    void set_right_value();
    std::string const &get_result_name() const;

    int64_t get_result_value();

    void evaluate(std::unique_ptr<formula::Formula> const &body,
                  std::vector<std::shared_ptr<util::Grounding>> &body_facts);
};

} // namespace laser::formula

#endif // LASER_FORMULA_CONDITION_ATOM_H
