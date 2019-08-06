//
// Created by mike on 7/17/18.
//

#ifndef LASER_EXAMPLE_EXAMPLE_IO_MANAGER_H
#define LASER_EXAMPLE_EXAMPLE_IO_MANAGER_H

#include <memory>
#include <vector>

#include <core/io_manager.h>
#include <util/grounding.h>
#include <util/timeline.h>

#include "example_parser.h"
#include "example_reader.h"
#include "example_writer.h"

namespace laser {
namespace example {

class ExampleIOManager : public laser::core::IOManager {
  private:
    ExampleWriter example_writer;
    ExampleReader example_stream_reader;
    ExampleParser example_parser;
    std::string latest_output;
    std::unordered_map<uint64_t, std::string> output_map;

  public:
    explicit ExampleIOManager(std::string stream_string);

    ~ExampleIOManager() override = default;

    uint64_t read_stream_start_time() override;

    uint64_t read_stream_end_time() override;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    read_stream_data(laser::util::Timeline &timeline) override;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    read_background_data() override;

    void
    write_output_data(uint64_t time,
                      std::vector<std::shared_ptr<laser::util::Grounding>>
                          output_vector) override;

    std::string get_output(uint64_t time) const;
};

} // namespace example 
} // namespace laser

#endif // LASER_EXAMPLE_EXAMPLE_IO_MANAGER_H
