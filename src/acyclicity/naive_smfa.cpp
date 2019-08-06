#include "acyclicity/naive_smfa.h"

namespace laser::acyclicity {

NaiveSMFA::NaiveSMFA(std::vector<rule::Rule> rule_vector)
    : rule_vector(std::move(rule_vector)) {}


bool NaiveSMFA::has_terminating_chase() {
    return false;
}

} // namespace laser::acyclicity
