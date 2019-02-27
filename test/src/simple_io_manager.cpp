#include <utility>

//
// Created by mike on 7/17/18.
//

#include "simple_io_manager.h"

uint64_t SimpleIOManager::read_stream_start_time() {
    if (! simple_stream_reader.has_metadata()) {
        simple_stream_reader.fetch_metadata();
    }
    return simple_stream_reader.get_stream_start_time();
}

uint64_t SimpleIOManager::read_stream_end_time() {
    if (! simple_stream_reader.has_metadata()) {
        simple_stream_reader.fetch_metadata();
    }
    return simple_stream_reader.get_stream_end_time();
}

std::vector<laser::util::DataAtom> SimpleIOManager::read_stream_data(
        uint64_t time) {
    auto data_vector = simple_stream_reader.read_next_data(time);
    auto parsed_data = simple_parser.parse_data(data_vector);
    return parsed_data;
}

std::vector<laser::util::DataAtom> SimpleIOManager::read_background_data() {
//    auto data_vector = simple_background_reader.read_all_data();
//    return simple_parser.parse_data(data_vector);
    return std::vector<laser::util::DataAtom>();
}

void SimpleIOManager::write_output_data(
        uint64_t time, std::vector<laser::util::DataAtom> const &output_vector) {
    latest_output = "";
    latest_output = simple_writer.format_output(time, output_vector);
    simple_writer.write_output(latest_output);
    output_map.try_emplace(time, latest_output);
}

std::string SimpleIOManager::get_output(uint64_t time) const {
    if (output_map.count(time) > 0) {
        return output_map.at(time);
    }
    return "";
}

SimpleIOManager::SimpleIOManager(std::string stream_string) {
    simple_stream_reader.set_source(std::move(stream_string));
}
