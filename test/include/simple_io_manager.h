//
// Created by mike on 7/17/18.
//

#ifndef TEST_SIMPLE_IO_MANAGER_H
#define TEST_SIMPLE_IO_MANAGER_H

#include "simple_parser.h"
#include "simple_reader.h"
#include "simple_writer.h"
#include <io/io_manager.h>

class SimpleIOManager : public laser::io::IOManager {
  private:
    SimpleWriter simple_writer;
    SimpleReader simple_stream_reader;
    SimpleParser simple_parser;
    std::string latest_output;

  public:
    explicit SimpleIOManager(std::string stream_string);

    ~SimpleIOManager() override = default;

    uint64_t read_stream_start_time() override;

    uint64_t read_stream_end_time() override;

    std::vector<laser::util::DataAtom> read_stream_data(uint64_t time) override;

    std::vector<laser::util::DataAtom> read_background_data() override;

    void write_output_data(
        uint64_t time,
        std::vector<laser::util::DataAtom> const &output_vector) override;

    std::string get_output() const;
};

#endif // TEST_SIMPLE_IO_MANAGER_H
