#ifndef LASER_RULE_MATH_EVALUATOR_VALUE_NODE_H
#define LASER_RULE_MATH_EVALUATOR_VALUE_NODE_H

#include <string>
#include <set>

namespace laser::rule {

struct value_node {
    int64_t num_value;
    uint64_t ht;
    uint64_t hc;
    std::string str_value;
};

struct value_node_compare {
    bool operator()(value_node const &x, value_node const &y) const {
        return x.num_value < y.num_value;
    }
};

using value_set = std::set<value_node, value_node_compare>;

} // namespace laser::rule

#endif // LASER_RULE_MATH_EVALUATOR_VALUE_NODE_H
