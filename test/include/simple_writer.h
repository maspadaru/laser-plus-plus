//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_WRITER_H
#define TEST_SIMPLE_WRITER_H

#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

#include <formula/grounding.h>
#include <util/data_atom.h>

class SimpleWriter {
  private:
    // methods

    std::vector<std::shared_ptr<laser::formula::Grounding>>
    remove_duplicates(std::vector<std::shared_ptr<laser::formula::Grounding>>
                          input_groundings) const;

  public:
    // constructors & destructors

    ~SimpleWriter() = default;

    // getters & setters

    // methods

    void write_output(std::string const &formatted_output_string) const;

    std::string
    format_output(uint64_t time,
                  std::vector<std::shared_ptr<laser::formula::Grounding>>
                      output_vector) const;
};

#endif // TEST_SIMPLE_FORMATTER_H
