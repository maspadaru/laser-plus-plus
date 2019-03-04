//
// Created by mike on 02/25/19.
//
#ifndef LASER_REASONER_REASONER_H
#define LASER_REASONER_REASONER_H

#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "io/io_manager.h"
#include "io/rule_reader.h"
#include "program/program.h"
#include "util/data_atom.h"
#include "util/timeline.h"

namespace laser {
namespace reasoner {

class Reasoner {
  private:
    std::unordered_map<uint64_t, std::vector<util::DataAtom>> fact_map;
    std::unordered_map<uint64_t, std::vector<util::DataAtom>> conclusion_map;
    std::mutex fact_map_mutex;
    std::mutex conclusion_map_mutex;
    io::IOManager *io_manager;
    io::RuleReader *rule_reader;
    double evaluate_secs = 0;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline);
    void write(util::Timeline timeline);

    void insert_facts(uint64_t timepoint, std::vector<util::DataAtom> &facts);
    void remove_facts(uint64_t timepoint);
    void insert_conclusions(uint64_t timepoint,
                            std::vector<util::DataAtom> &conclusions);
    void remove_conclusions(uint64_t timepoint);

  public:
    explicit Reasoner(io::RuleReader *rule_reader,
                                io::IOManager *io_manager);

    void start();
};

} // namespace reasoner
} // namespace laser
#endif // LASER_REASONER_REASONER_H
