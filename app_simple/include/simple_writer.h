//
// Created by mike on 03/07/18.
//

#ifndef SIMPLEAPP_SIMPLE_WRITER_H
#define SIMPLEAPP_SIMPLE_WRITER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>

#include <util/grounding.h>

class SimpleWriter {
  private:
    std::ofstream out;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    remove_duplicates(std::vector<std::shared_ptr<laser::util::Grounding>>
                          input_groundings) const;

  public:
    explicit SimpleWriter(std::string const &output_path);

    ~SimpleWriter();

    void write_output(std::string const &formatted_output_string);

    std::string
    format_output(uint64_t time,
                  std::vector<std::shared_ptr<laser::util::Grounding>>
                      output_vector) const;
};

#endif // SIMPLEAPP_SIMPLE_FORMATTER_H
