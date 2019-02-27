//
// Created by mike on 02/25/19.
//
#ifndef LASER_REASONER_SEQUENTIAL_REASONER_H
#define LASER_REASONER_SEQUENTIAL_REASONER_H

#include <unordered_map>
#include <vector>
#include <thread> 

#include "util/timeline.h"
#include "util/data_atom.h"
#include "io/io_manager.h"
#include "io/rule_reader.h"
#include "program/program.h"

namespace laser {
namespace reasoner {

class SequentialReasoner {
  private:
    std::unordered_map<uint64_t, std::vector<util::DataAtom>> fact_map;
    std::unordered_map<uint64_t, std::vector<util::DataAtom>> conclusion_map;
    io::IOManager *io_manager;
    io::RuleReader *rule_reader;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline);
    void write(util::Timeline timeline);

  public:
    explicit SequentialReasoner(io::RuleReader *rule_reader,
                       io::IOManager *io_manager);

    void start();
};

} // namespace reasoner
} // namespace laser
#endif // LASER_REASONER_SEQUENTIAL_REASONER_H
