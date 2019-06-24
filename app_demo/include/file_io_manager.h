#ifndef DEMOAPP_FILE_IO_MANAGER_H
#define DEMOAPP_FILE_IO_MANAGER_H

#include <memory>
#include <vector>

#include <core/io_manager.h>
#include <util/grounding.h>
#include <util/timeline.h>

#include "file_parser.h"
#include "file_reader.h"
#include "file_writer.h"

class FileIOManager : public laser::core::IOManager {
  private:
    FileWriter file_writer;
    FileReader file_stream_reader;
    FileParser file_parser;
    std::string latest_output;
    uint64_t start_time;
    uint64_t end_time;
    bool is_output_enabled;

  public:
    FileIOManager(std::string const &stream_path,
                  std::string const &output_path, uint64_t start_time,
                  uint64_t end_time, int facts_per_timepoint, bool is_output_enabled);

    ~FileIOManager() override = default;

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



    std::string get_output() const;
};

#endif // DEMOAPP_FILE_IO_MANAGER_H
