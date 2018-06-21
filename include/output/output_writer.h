//
// Created by mike on 6/17/18.
//

#ifndef LASER_OUTPUT_OUTPUT_WRITER_H
#define LASER_OUTPUT_OUTPUT_WRITER_H

#include <string>

namespace laser {
namespace output {


/**
 *  OutputWriter Interface
 */
class OutputWriter {
private:

public:
// constructors & destructors

    virtual ~OutputWriter() = default;

// getters & setters

// methods


    /**
     * @throw WriteException if any problem occurred while writing the output,
     * e.g.: target file is inaccessible
     */
    virtual void write_output(std::string formatted_output_string) = 0;

};

} // namespace output
} // namespace laser

#endif // LASER_OUTPUT_OUTPUT_WRITER_H
