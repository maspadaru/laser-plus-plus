#include <utility>

//
// Created by mike on 7/17/18.
//

#include "example/example_io_manager.h"

namespace laser {
namespace example {

uint64_t ExampleIOManager::read_stream_start_time() {
    if (!example_stream_reader.has_metadata()) {
        example_stream_reader.fetch_metadata();
    }
    return example_stream_reader.get_stream_start_time();
}

uint64_t ExampleIOManager::read_stream_end_time() {
    if (!example_stream_reader.has_metadata()) {
        example_stream_reader.fetch_metadata();
    }
    return example_stream_reader.get_stream_end_time();
}

std::vector<std::shared_ptr<laser::util::Grounding>>
ExampleIOManager::read_stream_data(laser::util::Timeline &timeline) {
    auto data_vector = example_stream_reader.read_next_data(timeline.get_time());
    auto parsed_data = example_parser.parse_data(timeline, data_vector);
    return parsed_data;
}

std::vector<std::shared_ptr<laser::util::Grounding>>
ExampleIOManager::read_background_data() {
    return std::vector<std::shared_ptr<laser::util::Grounding>>();
}

void ExampleIOManager::write_output_data(
    uint64_t time,
    std::vector<std::shared_ptr<laser::util::Grounding>> output_vector) {
    latest_output = "";
    latest_output = example_writer.format_output(time, std::move(output_vector));
    example_writer.write_output(latest_output);
    output_map.try_emplace(time, latest_output);
}

std::string ExampleIOManager::get_output(uint64_t time) const {
    if (output_map.count(time) > 0) {
        return output_map.at(time);
    }
    return "";
}

ExampleIOManager::ExampleIOManager(std::string stream_string) {
    example_stream_reader.set_source(std::move(stream_string));
}

} // namespace example 
} // namespace laser
