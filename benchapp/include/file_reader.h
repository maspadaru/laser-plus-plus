//
// Created by mike on 28-11-2018.
//

#ifndef BENCHAPP_FILE_READER_H
#define BENCHAPP_FILE_READER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <util/format_exception.h>
#include <util/read_exception.h>

class FileReader {
  private:
    static constexpr char LINE_KEY = 0;
    static constexpr char LINE_DELIMITER = 1;
    static constexpr char LINE_VALUE = 2;
    static constexpr char EXPECTED_LINE_DELIMITER = ':';

    std::ifstream in;

    bool read_line(std::string &line);

  public:
    explicit FileReader(std::string const &stream_path);
    ~FileReader();

    std::vector<std::string> read_all_data();

    std::vector<std::string> read_next_data(uint64_t request_time_point);
};

#endif // BENCHAPP_FILE_READER_H
