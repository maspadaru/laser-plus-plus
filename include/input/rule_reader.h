//
// Created by mike on 6/17/18.
//

#ifndef LASER_INPUT_RULE_READER_H
#define LASER_INPUT_RULE_READER_H


namespace laser {
namespace input {

/**
 *  RuleReader Interface
 */
class RuleReader {
private:
public:
// constructors & destructors
    virtual ~RuleReader() = default;
// getters & setters
// const methods
// methods

    /**
     * Reads all the rules from input as raw strings.
     * @return Vector of strings. Each element is the raw string representation
     * of a rule.
     * @throw ReadException if any problem occurred while reading the input,
     * e.g.: source file is inaccessible
     */
    virtual std::vector<std::string> read_rules() const = 0;

};

} // namespace input
} // namespace laser


#endif // LASER_INPUT_RULE_READER_H
