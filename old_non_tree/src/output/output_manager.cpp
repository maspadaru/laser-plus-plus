//
// Created by mike on 6/15/18.
//

#include <excetion/uninitialized_exception.h>
#include <utility>
#include "output/output_manager.h"

namespace laser {
namespace output {

// constructors & destructors

// getters & setters

// methods

void OutputManager::initialize_output_manager(
        OutputWriter *output_writer, OutputFormatter *output_formatter) {
    this->output_writer = output_writer;
    this->output_formatter = output_formatter;
    this->is_initialised_output_writer_m = true;
}

void OutputManager::write_output(
        std::unordered_map<std::string,
                std::vector<laser::formula::Formula *>> output_map) {
    if (!is_initialised_output_writer_m) {
        throw exception::UninitializedException("The output writer was not "
                                                "initialised. Call "
                                                "initialize_output_manager() "
                                                "before calling write_output()");
    }
    auto formatted_output_string = output_formatter->format_output(
            std::move(output_map));
    output_writer->write_output(formatted_output_string);
}


} // namespace output
} // namespace laser
