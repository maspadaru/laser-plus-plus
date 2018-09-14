//
// Created by mike on 30.07.2018.
//

#include "program/io_handler.h"

namespace laser {
namespace program {

laser::program::IOHandler::IOHandler(io::IOManager *ioManager)
    : ioManager(ioManager) {}

std::unordered_map<std::string, std::vector<formula::Grounding>>
laser::program::IOHandler::get_stream_data(uint64_t time) {
    std::unordered_map<std::string, std::vector<formula::Grounding>> result;
    auto data_vector = ioManager->read_stream_data(time);
    for (auto const &data : data_vector) {
        std::string predicate = data.get_predicate();
        result.try_emplace(predicate);
        std::vector<formula::Grounding> &map_vector = result.at(predicate);
        // TODO !!! SET TUPLE COUNTER VALUES !!!!
        formula::Grounding grounding(time, time, 0, 9999);
        grounding.set_as_fact();
        grounding.add_substitution_vector(data.get_arguments());
        map_vector.push_back(grounding);
    }
    return result;
}

void IOHandler::put_conclusions(
    uint64_t time, std::vector<formula::Formula *> const &conclusions) {
    std::vector<util::DataAtom> data_vector;
    for (auto formula : conclusions) {
        std::string predicate = formula->get_predicate_vector().at(0);
        auto variable_names = formula->get_full_variable_names();
        size_t number_variables = variable_names.size();
        for (auto const &grounding : formula->get_groundings()) {
            std::vector<std::string> argument_vector;
            for (auto variable_name : variable_names) {
               size_t variable_index = formula->get_variable_index(variable_name);
               std::string argument = grounding.get_substitution(variable_index);
               argument_vector.push_back(argument);
            }
            auto data_atom = util::DataAtom(predicate, argument_vector);
            data_vector.push_back(data_atom);
        }
    }
    ioManager->write_output_data(time, data_vector);
}

} // namespace program
} // namespace laser
