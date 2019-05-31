#include <utility>

//
// Created by mike on 28-11-2018.
//

#include "file_io_manager.h"
#include <iostream>

FileIOManager::FileIOManager(std::string const &stream_path,
                             std::string const &output_path,
                             uint64_t start_time, uint64_t end_time,
                             int facts_per_timepoint, bool is_output_enabled)
    : file_stream_reader(FileReader(stream_path, facts_per_timepoint)),
      file_writer(FileWriter(output_path)),
      is_output_enabled(is_output_enabled) {
    this->start_time = start_time;
    this->end_time = end_time;
}

uint64_t FileIOManager::read_stream_start_time() { return start_time; }

uint64_t FileIOManager::read_stream_end_time() { return end_time; }

std::vector<std::shared_ptr<laser::util::Grounding>>
FileIOManager::read_stream_data(laser::util::Timeline &timeline) {
    auto time = timeline.get_time();
    auto data_vector = file_stream_reader.read_next_data(time);
    auto parsed_data = file_parser.parse_data(timeline, data_vector);
    return parsed_data;
}

std::vector<std::shared_ptr<laser::util::Grounding>>
FileIOManager::read_background_data() {
    return std::vector<std::shared_ptr<laser::util::Grounding>>();
}

void FileIOManager::write_output_data(
    uint64_t time, std::vector<std::shared_ptr<laser::util::Grounding>> 
                       output_vector) {
    if (is_output_enabled) {
        latest_output = "";
        latest_output = file_writer.format_output(time, std::move(output_vector));
        file_writer.write_output(latest_output);
        //std::cerr << "Should write output "<< latest_output << std::endl << std::endl;
    }
}

std::string FileIOManager::get_output() const { return latest_output; }
