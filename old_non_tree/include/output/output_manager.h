//
// Created by mike on 6/15/18.
//
#ifndef LASER_OUTPUT_OUTPUT_MANAGER_H
#define LASER_OUTPUT_OUTPUT_MANAGER_H

#include "output_formatter.h"
#include "output_writer.h"

namespace laser {
namespace output {

class OutputManager {
private:

    OutputWriter *output_writer = nullptr;
    OutputFormatter *output_formatter = nullptr;

    bool is_initialised_output_writer_m = false;

public:
// constructors & destructors

    OutputManager() = default;

    ~OutputManager() = default;

// getters & setters

// methods
    void initialize_output_manager(
            OutputWriter *output_writer,
            OutputFormatter *output_formatter);


    /**
    *
    * @param output_map Map predicate to vector of intensional facts
    * @throw WriteException if any problem occurred while writing the output,
    *  e.g.: target file is inaccessible
    * @throw FormatException if the output cannot be properly formatted.
    * @throw UninitializedException if the output writer was not
    *      initialised by calling initialize_output_manager() before calling
    *      write_output().
    */
    void write_output(
            std::unordered_map<std::string,
                    std::vector<laser::formula::Formula *>> output_map);

};

} // namespace output
} // namespace laser



#endif // LASER_OUTPUT_OUTPUT_MANAGER_H
