// Created by mike on 6/21/18.
//

#include "file_writer.h"

FileWriter::FileWriter(std::string const &output_path) {
    out.open(output_path);
}

FileWriter::~FileWriter() { out.close(); }

std::vector<std::shared_ptr<laser::util::Grounding>>
FileWriter::remove_duplicates(
    std::vector<std::shared_ptr<laser::util::Grounding>> input_groundings)
    const {
    std::set<std::shared_ptr<laser::util::Grounding>,
             laser::util::GroundingPredicateSubstitutionCompare>
        grounding_set;
    for (auto &grounding : input_groundings) {
        grounding_set.insert(std::move(grounding));
    }
    std::vector<std::shared_ptr<laser::util::Grounding>> result;
    result.insert(result.end(), std::make_move_iterator(grounding_set.begin()),
                  std::make_move_iterator(grounding_set.end()));
    return result;
}

std::string FileWriter::format_output(
    uint64_t time,
    std::vector<std::shared_ptr<laser::util::Grounding>> output_vector) const {
    std::cerr << "Writing:" << output_vector.size() << std::endl << std::endl;
    std::stringstream result_stream;
    const std::string TIME_SEPARATOR = " -> ";
    const std::string ARGUMENTS_START = "(";
    const std::string ARGUMENTS_END = ")";
    const std::string DELIMITER = ", ";
    result_stream << time << TIME_SEPARATOR;
    auto unique_vector = remove_duplicates(std::move(output_vector));
    for (size_t atom_index = 0; atom_index < unique_vector.size();
         atom_index++) {
        auto const &data_atom = unique_vector.at(atom_index);
        result_stream << data_atom->get_predicate() << ARGUMENTS_START;
        auto argument_vector = data_atom->get_constant_vector();
        for (size_t argument_index = 0; argument_index < argument_vector.size();
             argument_index++) {
            auto const &argument = argument_vector.at(argument_index);
            result_stream << argument;
            if (argument_index < argument_vector.size() - 1) {
                result_stream << DELIMITER;
            }
        }
        result_stream << ARGUMENTS_END;
        if (atom_index < unique_vector.size() - 1) {
            result_stream << DELIMITER;
        }
    }
    auto result = result_stream.str();
    return result;
}

void FileWriter::write_output(std::string const &formatted_output_string) {
    out << formatted_output_string << std::endl;
}
