//
// Created by mike on 02/25/19.
//
#ifndef LASER_CORE_REASONER_H
#define LASER_CORE_REASONER_H

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "core/io_manager.h"
#include "core/program.h"
#include "rule/rule_reader.h"
#include "util/grounding.h"
#include "util/settings.h"
#include "util/timeline.h"
#include "util/chase_algorithm.h"

namespace laser {
namespace core {

class Reasoner {
  private:
    std::mutex fact_map_mutex;
    std::mutex conclusion_map_mutex;
    IOManager *io_manager;
    rule::RuleReader *rule_reader;
    std::chrono::duration<double, std::milli> clock_eval;

    std::unordered_map<uint64_t, std::vector<std::shared_ptr<util::Grounding>>>
        fact_map;

    std::unordered_map<uint64_t, std::vector<std::shared_ptr<util::Grounding>>>
        conclusion_map;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline);
    void write(util::Timeline timeline);

    void insert_facts(uint64_t timepoint,
                      std::vector<std::shared_ptr<util::Grounding>> facts);

    std::vector<std::shared_ptr<util::Grounding>> get_facts(uint64_t timepoint);

    void insert_conclusions(
        uint64_t timepoint,
        std::vector<std::shared_ptr<util::Grounding>> conclusions);

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions(uint64_t timepoint);

  public:
    explicit Reasoner(rule::RuleReader *rule_reader, IOManager *io_manager,
                      util::ChaseAlgorithm chase_algorithm);

    void start();
};

} // namespace core
} // namespace laser
#endif // LASER_CORE_REASONER_H
