//
// Created by mike on 28-11-2018.
//

#ifndef BENCHAPP_FILE_IO_MANAGER_H
#define BENCHAPP_FILE_IO_MANAGER_H

#include "file_parser.h"
#include "file_reader.h"
#include "file_writer.h"
#include <io/io_manager.h>

class FileIOManager : public laser::io::IOManager {
  private:
    FileWriter file_writer;
    FileReader file_stream_reader;
    FileParser file_parser;
    std::string latest_output;
    uint64_t start_time;
    uint64_t end_time;

  public:
    FileIOManager(std::string const &stream_path,
                  std::string const &output_path, uint64_t start_time,
                  uint64_t end_time);

    ~FileIOManager() override = default;

    uint64_t read_stream_start_time() override;

    uint64_t read_stream_end_time() override;

    std::vector<laser::util::DataAtom> read_stream_data(uint64_t time) override;

    std::vector<laser::util::DataAtom> read_background_data() override;

    void write_output_data(
        uint64_t time,
        std::vector<laser::util::DataAtom> output_vector) override;

    std::string get_output() const;
};

#endif // BENCHAPP_FILE_IO_MANAGER_H
