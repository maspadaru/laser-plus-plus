//
// Created by mike on 02/25/19.
//
#ifndef LASER_REASONER_REASONER_H
#define LASER_REASONER_REASONER_H

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "formula/grounding.h"
#include "io/io_manager.h"
#include "io/rule_reader.h"
#include "program/program.h"
#include "util/timeline.h"

namespace laser {
namespace reasoner {

class Reasoner {
  private:
    std::mutex fact_map_mutex;
    std::mutex conclusion_map_mutex;
    io::IOManager *io_manager;
    io::RuleReader *rule_reader;
    std::chrono::duration<double, std::milli> clock_eval;

    std::unordered_map<uint64_t,
                       std::vector<std::shared_ptr<formula::Grounding>>>
        fact_map;

    std::unordered_map<uint64_t,
                       std::vector<std::shared_ptr<formula::Grounding>>>
        conclusion_map;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline);
    void write(util::Timeline timeline);

    void insert_facts(uint64_t timepoint,
                      std::vector<std::shared_ptr<formula::Grounding>> facts);

    std::vector<std::shared_ptr<formula::Grounding>>
    get_facts(uint64_t timepoint);

    void insert_conclusions(
        uint64_t timepoint,
        std::vector<std::shared_ptr<formula::Grounding>> conclusions);

    std::vector<std::shared_ptr<formula::Grounding>>
    get_conclusions(uint64_t timepoint);

  public:
    explicit Reasoner(io::RuleReader *rule_reader, io::IOManager *io_manager);

    void start();
};

} // namespace reasoner
} // namespace laser
#endif // LASER_REASONER_REASONER_H
