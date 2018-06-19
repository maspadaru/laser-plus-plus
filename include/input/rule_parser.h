//
// Created by mike on 6/17/18.
//

#ifndef LASER_INPUT_RULE_PARSER_H
#define LASER_INPUT_RULE_PARSER_H


#include <string>
#include <vector>

namespace laser {
namespace input {

/**
 *  RuleParser Interface
 */
class RuleParser {
private:
public:
// constructors & destructors
    virtual ~RuleParser() = default;
// getters & setters
// const methods

    /**
    * Parses the rules from the raw strings.
    *
    * @param raw_data Vector of strings. Each element contains the raw string
    * of a rule that must be parsed
    * @return Vector of pointers to Rule objects obtained by parsing the raw
    * rule strings
    * @throw FormatException if the input is not in a format that can be
    * properly parsered.
    */
    virtual std::vector<rule::Rule *>
    parse_rules(std::vector<std::string> raw_rule_vector) const = 0;

// methods

};

} // namespace input
} // namespace laser

#endif // LASER_INPUT_RULE_PARSER_H
