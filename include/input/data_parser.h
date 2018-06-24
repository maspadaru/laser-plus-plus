//
// Created by mike on 6/15/18.
//
#ifndef LASER_INPUT_DATA_PARSER_H
#define LASER_INPUT_DATA_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>
#include "formula/formula.h"

namespace laser {
namespace input {

/**
 *  DataParser Interface
 */

class DataParser {
private:
public:
// constructors & destructors
    virtual ~DataParser() = default;
// getters & setters
// const methods
// methods

    /**
    * Parses the facts from the raw data stream.
    * @param raw_data string containing the raw data that must be parsed
    * @return
     *      Tuple: [1] Number of parsed facts.
 *                 [2] Unordered Map where the keys are predicates and the
    *             values are vectors containing the facts that have the
    *             corresponding key as predicate.
    * @throw FormatException if the input is not in a format that can be
    *      properly parsed.
    */
    virtual std::tuple<int, std::unordered_map<std::string,
                    std::vector<laser::formula::Formula *>>>
    parse_data(std::vector<std::string> raw_data_vector) = 0;

};


} // namespace input
} // namespace laser



#endif // LASER_INPUT_DATA_PARSER_H
