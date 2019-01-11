//
// Created by mike on 6/21/18.
//

#ifndef BENCHAPP_FILE_PARSER_H
#define BENCHAPP_FILE_PARSER_H

#include <algorithm>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <util/data_atom.h>
#include <util/format_exception.h>
class FileParser {
  private:
    laser::util::DataAtom parse_string(std::string const &input);

  public:
    ~FileParser() = default;

    std::vector<laser::util::DataAtom>
    parse_data(std::vector<std::string> raw_data_vector);
};

#endif // BENCHAPP_FILE_PARSER_H
