//
// Created by mike on 6/21/18.
//

#include "simple_writer.h"

// constructors & destructors

// getters & setters

// methods

std::vector<std::shared_ptr<laser::formula::Grounding>>
SimpleWriter::remove_duplicates(
    std::vector<std::shared_ptr<laser::formula::Grounding>> &input_groundings)
    const {
    std::set<std::shared_ptr<laser::formula::Grounding>,
             laser::formula::GroundingPredicateSubstitutionCompare>
        grounding_set;
    for (auto &grounding : input_groundings) {
        grounding_set.insert(std::move(grounding));
    }
    std::vector<std::shared_ptr<laser::formula::Grounding>> result;
    result.insert(result.end(), std::make_move_iterator(grounding_set.begin()),
                  std::make_move_iterator(grounding_set.end()));
    return result;
}

std::string SimpleWriter::format_output(
    uint64_t time,
    std::vector<std::shared_ptr<laser::formula::Grounding>> output_vector)
    const {
    std::stringstream result_stream;
    const std::string TIME_SEPARATOR = " -> ";
    const std::string ARGUMENTS_START = "(";
    const std::string ARGUMENTS_END = ")";
    const std::string ARGUMENT_DELIMITER = ", ";
    const std::string CONCLUSION_DELIMITER = " ";
    result_stream << time << TIME_SEPARATOR;
    auto unique_vector = remove_duplicates(output_vector);
    for (auto const &data_atom : unique_vector) {
        result_stream << data_atom->get_predicate() << ARGUMENTS_START;
        auto argument_vector = data_atom->get_constant_vector();
        for (size_t argument_index = 0; argument_index < argument_vector.size();
             argument_index++) {
            auto const &argument = argument_vector.at(argument_index);
            result_stream << argument;
            if (argument_index < argument_vector.size() - 1) {
                result_stream << ARGUMENT_DELIMITER;
            }
        }
        result_stream << ARGUMENTS_END;
        result_stream << CONCLUSION_DELIMITER;
    }
    return result_stream.str();
}

void SimpleWriter::write_output(std::string const &formatted_output_string) const {
    std::cout << formatted_output_string << std::endl;
}
