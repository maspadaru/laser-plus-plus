//
// Created by mike on 7/17/18.
//

#ifndef TEST_SIMPLE_IO_MANAGER_H
#define TEST_SIMPLE_IO_MANAGER_H

#include <memory>
#include <vector>

#include <io/io_manager.h>
#include <formula/grounding.h>
#include <util/timeline.h>

#include "simple_parser.h"
#include "simple_reader.h"
#include "simple_writer.h"

class SimpleIOManager : public laser::io::IOManager {
  private:
    SimpleWriter simple_writer;
    SimpleReader simple_stream_reader;
    SimpleParser simple_parser;
    std::string latest_output;
    std::unordered_map<uint64_t, std::string> output_map;

  public:
    explicit SimpleIOManager(std::string stream_string);

    ~SimpleIOManager() override = default;

    uint64_t read_stream_start_time() override;

    uint64_t read_stream_end_time() override;

    std::vector<std::shared_ptr<laser::formula::Grounding>>
    read_stream_data(laser::util::Timeline &timeline) override;

    std::vector<std::shared_ptr<laser::formula::Grounding>>
    read_background_data() override;

    void
    write_output_data(uint64_t time,
                      std::vector<std::shared_ptr<laser::formula::Grounding>>
                          output_vector) override;

    std::string get_output(uint64_t time) const;
};

#endif // TEST_SIMPLE_IO_MANAGER_H
