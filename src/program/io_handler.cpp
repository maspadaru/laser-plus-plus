//
// Created by mike on 30.07.2018.
//

#include "program/io_handler.h"

namespace laser {
namespace program {

laser::program::IOHandler::IOHandler(io::IOManager *ioManager)
    : ioManager(ioManager) {}

std::unordered_map<std::string,
                   std::vector<std::shared_ptr<formula::Grounding>>>
laser::program::IOHandler::get_stream_data(uint64_t time) {
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
        result;

    //clock_t begin = clock();
    auto data_vector = ioManager->read_stream_data(time);
    //clock_t end_read = clock();
    //double elapsed_secs = double(end_read - begin) / CLOCKS_PER_SEC;
    //reader_secs += elapsed_secs;

    for (auto const &data : data_vector) {
        std::string predicate = data.get_predicate();
        result.try_emplace(predicate);
        std::vector<std::shared_ptr<formula::Grounding>> &map_vector =
            result.at(predicate);
        // TODO !!! SET TUPLE COUNTER VALUES !!!!
        auto grounding = std::make_shared<laser::formula::Grounding>(
            time, time, 0, 9999, true, false, data.get_arguments());
        map_vector.push_back(grounding);
    }
    //double end = clock();
    //elapsed_secs = double(end - end_read) / CLOCKS_PER_SEC;
    //handler_secs += elapsed_secs;
    return result;
}

//double IOHandler::get_reader_secs() const {
    //return reader_secs;

//}

//double IOHandler::get_handler_secs() const {
   //return handler_secs; 
//}

void IOHandler::put_conclusions(
    util::Timeline timeline,
    std::vector<formula::Formula *> const &conclusions) {
    std::vector<util::DataAtom> data_vector;
    for (auto const formula : conclusions) {
        std::string predicate = formula->get_predicate_vector().at(0);
        auto variable_names = formula->get_full_variable_names();
        size_t number_variables = variable_names.size();
        for (auto const &grounding :
             formula->get_conclusions_timepoint(timeline)) {
            std::vector<std::string> argument_vector;
            for (auto const &variable_name : variable_names) {
                size_t variable_index =
                    formula->get_variable_index(variable_name);
                std::string argument = grounding->get_constant(variable_index);
                argument_vector.push_back(argument);
            }
            auto data_atom = util::DataAtom(predicate, argument_vector);
            data_vector.push_back(data_atom);
        }
    }
    ioManager->write_output_data(timeline.get_time(), data_vector);
}

} // namespace program
} // namespace laser
