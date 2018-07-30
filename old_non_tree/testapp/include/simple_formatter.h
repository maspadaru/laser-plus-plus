//
// Created by mike on 6/21/18.
//

#ifndef TEST_SIMPLE_FORMATTER_H
#define TEST_SIMPLE_FORMATTER_H

#include <output/output_formatter.h>
#include <formula/formula.h>

class SimpleFormatter : public laser::output::OutputFormatter {
private:

// methods

public:

// constructors & destructors

    ~SimpleFormatter() = default;

// getters & setters

// methods

    std::string format_output(
            std::unordered_map<std::string,
                    std::vector<laser::formula::Formula *>> output_map) const override;
};

#endif // TEST_SIMPLE_FORMATTER_H