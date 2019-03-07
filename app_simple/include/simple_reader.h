//
// Created by mike on 03/07/18.
//

#ifndef SIMPLEAPP_SIMPLE_READER_H
#define SIMPLEAPP_SIMPLE_READER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <util/read_exception.h>
#include <util/format_exception.h>


class SimpleReader {
private:

    static constexpr char LINE_KEY = 0;
    static constexpr char LINE_DELIMITER = 1;
    static constexpr char LINE_VALUE = 2;
    static constexpr char EXPECTED_LINE_DELIMITER = ':';

    bool has_source = false;
    uint64_t start_time;
    uint64_t end_time;
    bool has_metadata_m = false;
    std::string latest_read_line;

    std::ifstream source;
    bool read_line(std::string &line);

    bool read_line();

    std::unordered_map<char, std::string> parse_latest_line() const;

    bool is_valid_line(
            std::string key, std::string delimiter, std::string value) const;

    std::vector<std::string> read_all_lines();

public:
    SimpleReader(std::string const &stream_path);
    ~SimpleReader();

    bool has_metadata() const;

    bool fetch_metadata();

    uint64_t get_stream_start_time() const;

    uint64_t get_stream_end_time() const;

    std::vector<std::string> read_all_data();

    std::vector<std::string>
    read_next_data(uint64_t request_time_point);

};


#endif // SIMPLEAPP_SIMPLE_READER_H
