//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_WRITER_H
#define TEST_SIMPLE_WRITER_H

#include <iostream>
#include <sstream>

#include <util/data_atom.h>

class SimpleWriter {
private:

// methods

public:

// constructors & destructors

    ~SimpleWriter() = default;

// getters & setters

// methods

    void write_output(std::string formatted_output_string) const;

    std::string format_output(uint64_t time,
            std::vector<laser::util::DataAtom> output_vector) const;
};

#endif // TEST_SIMPLE_FORMATTER_H