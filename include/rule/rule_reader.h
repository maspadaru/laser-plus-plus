//
// Created by mike on 7/17/18.
//

#ifndef LASER_RULE_RULE_READER_H
#define LASER_RULE_RULE_READER_H

#include "rule/rule.h"

namespace laser {
namespace rule {

/**
 *
 */
class RuleReader {
public:

// constructors & destructors

    virtual ~RuleReader() = default;

// getters & setters

// methods

    /**
    *
    * @return Vector of Rule objects
    * @throw FormatException if the input is not in a format that can be
    * properly parsed.
    * @throw ReadException if any problem occurred while reading the input,
    * e.g.: source file is inaccessible
    */
    virtual std::vector<rule::Rule> get_rules() = 0;
};

} // namespace rule 
} // namespace laser

#endif // LASER_RULE_RULE_READER_H
