// Created by mike on 6/21/18.
//

#ifndef BENCHAPP_FILE_WRITER_H
#define BENCHAPP_FILE_WRITER_H

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include <formula/grounding.h>

class FileWriter {
  private:
    std::ofstream out;

    // methods

    std::vector<std::shared_ptr<laser::formula::Grounding>>
    remove_duplicates(std::vector<std::shared_ptr<laser::formula::Grounding>>
                          input_groundings) const;

  public:
    // constructors & destructors

    explicit FileWriter(std::string const &output_path);
    ~FileWriter();

    // getters & setters

    // methods

    void write_output(std::string const &formatted_output_string);

    std::string
    format_output(uint64_t time,
                  std::vector<std::shared_ptr<laser::formula::Grounding>>
                      output_vector) const;
};

#endif // BENCHAPP_FILE_WRITER_H
