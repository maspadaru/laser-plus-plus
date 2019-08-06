#ifndef LASER_ACYCLICITY_NAIVE_SMFA_H
#define LASER_ACYCLICITY_NAIVE_SMFA_H

#include <vector>

#include "core/program.h"
#include "rule/rule.h"
#include "util/timeline.h"

namespace laser::acyclicity {

class NaiveSMFA {
  private:
    std::vector<rule::Rule> rule_vector;

  public:
    explicit NaiveSMFA(std::vector<rule::Rule> rule_vector);
    bool has_terminating_chase();
};

} // namespace laser::acyclicity

#endif // LASER_ACYCLICITY_NAIVE_SMFA_H
