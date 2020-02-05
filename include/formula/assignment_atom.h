#ifndef LASER_FORMULA_ASSIGNMENT_ATOM_H
#define LASER_FORMULA_ASSIGNMENT_ATOM_H

#include <memory>
#include <string>
#include <vector>

#include "formula/formula.h"
#include "util/grounding.h"

namespace laser::formula {

class AssignmentAtom {
  private:
    std::string result_variable;
    std::string value;

  public:
    AssignmentAtom(std::string result_variable, std::string value);

    ~AssignmentAtom() = default;

    std::string const &get_result_name() const;

    int64_t get_value();

    void evaluate(std::unique_ptr<formula::Formula> const &body,
                  std::vector<std::shared_ptr<util::Grounding>> &body_facts);
};

} // namespace laser::formula

#endif // LASER_FORMULA_ASSIGNMENT_ATOM_H
