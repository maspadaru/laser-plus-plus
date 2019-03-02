//
// Created by mike on 02/25/19.
//
#ifndef LASER_REASONER_SEQUENTIAL_REASONER_H
#define LASER_REASONER_SEQUENTIAL_REASONER_H

#include <vector>
#include <ctime>
#include <iostream>

#include "io/io_manager.h"
#include "io/rule_reader.h"
#include "program/program.h"
#include "util/data_atom.h"
#include "util/timeline.h"

namespace laser {
namespace reasoner {

class SequentialReasoner {
  private:
    std::vector<util::DataAtom> facts;
    std::vector<util::DataAtom> conclusions;
    io::IOManager *io_manager;
    io::RuleReader *rule_reader;
    double read_secs = 0;
    double evaluate_secs = 0;
    double write_secs = 0;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline, program::Program &program);
    void write(util::Timeline timeline);

  public:
    explicit SequentialReasoner(io::RuleReader *rule_reader,
                                io::IOManager *io_manager);

    void start();
};

} // namespace reasoner
} // namespace laser
#endif // LASER_REASONER_SEQUENTIAL_REASONER_H
