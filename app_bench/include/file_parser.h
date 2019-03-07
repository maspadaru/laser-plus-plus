//
// Created by mike on 6/21/18.
//

#ifndef BENCHAPP_FILE_PARSER_H
#define BENCHAPP_FILE_PARSER_H

#include <algorithm>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <util/grounding.h>
#include <util/format_exception.h>
#include <util/timeline.h>

class FileParser {
  private:
    uint64_t current_tuple_counter = 0;
    std::shared_ptr<laser::util::Grounding>
    parse_string(laser::util::Timeline const &timeline, std::string const &input);

  public:
    ~FileParser() = default;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    parse_data(laser::util::Timeline &timeline,
               std::vector<std::string> const &raw_data_vector);
};

#endif // BENCHAPP_FILE_PARSER_H
