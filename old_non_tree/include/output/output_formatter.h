//
// Created by mike on 6/17/18.
//

#ifndef LASER_OUTPUT_OUTPUT_FORMATER_H
#define LASER_OUTPUT_OUTPUT_FORMATER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "formula/formula.h"

namespace laser {
namespace output {


/**
 *  Formatter Interface
 */
class OutputFormatter {
private:

public:
// constructors & destructors

    virtual ~OutputFormatter() = default;

// getters & setters

// methods


    /**
     *
    * @throw FormatException if the output cannot be properly formatted.
    */
    virtual std::string format_output(
            std::unordered_map<std::string,
                    std::vector<laser::formula::Formula *>> output_map) const = 0;

};

} // namespace output
} // namespace laser

#endif // LASER_OUTPUT_OUTPUT_FORMATER_H
