#include <utility>

//
// Created by mike on 7/17/18.
//

#include <simple_io_manager.h>

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

std::vector<laser::io::DataAtom> SimpleIOManager::read_stream_data(
        uint64_t time) {
    auto data_vector = simple_stream_reader.read_next_data(time);
    return simple_parser.parse_data(data_vector);
}

std::vector<laser::io::DataAtom> SimpleIOManager::read_background_data() {
//    auto data_vector = simple_background_reader.read_all_data();
//    return simple_parser.parse_data(data_vector);
    return std::vector<laser::io::DataAtom>();
}

void SimpleIOManager::write_output_data(
        uint64_t time, std::vector<laser::io::DataAtom> output_vector) {
    std::string formated_output = simple_writer.format_output(time, output_vector);
    simple_writer.write_output(formated_output);
}

SimpleIOManager::SimpleIOManager(std::string stream_string) {
    simple_stream_reader.set_source(std::move(stream_string));
}