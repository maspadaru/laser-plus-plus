//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_READER_H
#define TEST_SIMPLE_READER_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include <excetion/read_exception.h>
#include <excetion/format_exception.h>

#include <input/data_reader.h>
#include <input/rule_reader.h>

class SimpleReader
        : public laser::input::DataReader, public laser::input::RuleReader {
private:

    static constexpr char LINE_KEY = 0;
    static constexpr char LINE_DELIMITER = 1;
    static constexpr char LINE_VALUE = 2;
    static constexpr char EXPECTED_LINE_DELIMITER = ':';

    std::stringstream source;
    bool has_source = false;
    uint64_t start_time;
    uint64_t end_time;
    bool has_metadata_m = false;
    std::string latest_read_line;

// methods

    void check_source() const;

    bool read_line();

    std::unordered_map<char, std::string> parse_latest_line() const;

    bool is_valid_line(
            std::string key, std::string delimiter, std::string value) const;

    std::vector<std::string> read_all_lines();

public:
    ~SimpleReader() override = default;

// from laser::input::DataReader

    bool has_metadata() const override;

    uint64_t get_stream_start_time() const override;

    uint64_t get_stream_end_time() const override;

    std::vector<std::string> read_all_data() override;

    std::tuple<uint64_t, std::vector<std::string>>
    read_next_data(uint64_t request_time_point) override;

    bool fetch_metadata() override;


// laser::input::RuleReader

    std::vector<std::string> read_rules() override;

// Own Methods:

    void set_source(std::string source_string);

};


#endif // TEST_SIMPLE_READER_H

/*
 * Problem: latest_read_line will reset when calling read_all_data() after
 * read_next_data(). But no one should ever do that...
 */