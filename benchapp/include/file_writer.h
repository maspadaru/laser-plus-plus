// Created by mike on 6/21/18.
//

#ifndef BENCHAPP_FILE_WRITER_H
#define BENCHAPP_FILE_WRITER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <util/data_atom.h>

class FileWriter {
private:
    std::ofstream out;

// methods

public:

// constructors & destructors

    explicit FileWriter(std::string const &output_path);
    ~FileWriter();

// getters & setters

// methods

    void write_output(std::string const &formatted_output_string);

    std::string format_output(uint64_t time,
            std::vector<laser::util::DataAtom> output_vector) const;
};

#endif // BENCHAPP_FILE_WRITER_H
