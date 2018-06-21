//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_WRITER_H
#define TEST_SIMPLE_WRITER_H

#include <output/output_writer.h>

class SimpleWriter : public laser::output::OutputWriter {
private:

// methods

public:

// constructors & destructors

    ~SimpleWriter() = default;

// getters & setters

// methods

    void write_output(std::string formatted_output_string) override;
};

#endif // TEST_SIMPLE_FORMATTER_H