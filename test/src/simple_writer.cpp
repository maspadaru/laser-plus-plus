//
// Created by mike on 6/21/18.
//

#include "simple_writer.h"

// constructors & destructors

// getters & setters

// methods

std::string SimpleWriter::format_output(
    uint64_t time, std::vector<laser::util::DataAtom> output_vector) const {
    std::stringstream result_stream;
    const std::string TIME_SEPARATOR = " -> ";
    const std::string ARGUMENTS_START = "(";
    const std::string ARGUMENTS_END = ")";
    const std::string DELIMITER = ", ";
    result_stream << time << TIME_SEPARATOR;
    for (size_t atom_index = 0; atom_index < output_vector.size();
         atom_index++) {
        auto const &data_atom = output_vector.at(atom_index);
        result_stream << *data_atom.get_predicate() << ARGUMENTS_START;
        auto argument_vector = data_atom.get_arguments();
        for (size_t argument_index = 0; argument_index < argument_vector.size();
             argument_index++) {
            auto argument = argument_vector.at(argument_index);
            result_stream << *argument;
            if (argument_index < argument_vector.size() - 1) {
                result_stream << DELIMITER;
            }
        }
        result_stream << ARGUMENTS_END;
        if (atom_index < output_vector.size() - 1) {
            result_stream << DELIMITER;
        }
    }
    return result_stream.str();
}

void SimpleWriter::write_output(std::string formatted_output_string) const {
    std::cout << formatted_output_string << std::endl;
}
