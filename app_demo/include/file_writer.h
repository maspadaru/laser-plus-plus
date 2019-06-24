// Created by mike on 6/21/18.
//

#ifndef BENCHAPP_FILE_WRITER_H
#define BENCHAPP_FILE_WRITER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>

#include <util/grounding.h>

class FileWriter {
  private:
    std::ofstream out;

    std::vector<std::shared_ptr<laser::util::Grounding>>
    remove_duplicates(std::vector<std::shared_ptr<laser::util::Grounding>>
                          input_groundings) const;

  public:
    explicit FileWriter(std::string const &output_path);
    ~FileWriter();

    void write_output(std::string const &formatted_output_string);

    std::string
    format_output(uint64_t time,
                  std::vector<std::shared_ptr<laser::util::Grounding>>
                      output_vector) const;
};

#endif // BENCHAPP_FILE_WRITER_H
