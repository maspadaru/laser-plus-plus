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
#include "rule/rule.h"
//#include "service/service_manager.h"
#include "util/chase_algorithm.h"
#include "util/grounding.h"
#include "util/settings.h"
#include "util/timeline.h"

namespace laser::core {

class Reasoner {
  private:
    std::mutex fact_map_mutex;
    std::mutex conclusion_map_mutex;
    IOManager *io_manager;
    // service::ServiceManager *service_manager;
    std::vector<std::unique_ptr<laser::rule::Rule>> *rule_vector;
    bool is_listen_on = false;
    std::chrono::duration<double, std::milli> clock_eval;

    std::unordered_map<uint64_t, std::vector<std::shared_ptr<util::Grounding>>>
        fact_map;

    std::unordered_map<uint64_t, std::vector<std::shared_ptr<util::Grounding>>>
        conclusion_map;

    void read(util::Timeline timeline);
    void evaluate(util::Timeline timeline);
    void write(util::Timeline timeline);
    void listen(util::Timeline timeline);

    void insert_facts(uint64_t timepoint,
                      std::vector<std::shared_ptr<util::Grounding>> facts);

    std::vector<std::shared_ptr<util::Grounding>> get_facts(uint64_t timepoint);

    void insert_conclusions(
        uint64_t timepoint,
        std::vector<std::shared_ptr<util::Grounding>> conclusions);

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions(uint64_t timepoint);

  public:
    explicit Reasoner(
        std::vector<std::unique_ptr<laser::rule::Rule>> *rule_vector,
        IOManager *io_manager);

    // explicit Reasoner(std::vector<rule::Rule> *rule_vector,
    // IOManager *io_manager,
    // service::ServiceManager *service_manager);

    double start();
};

} // namespace laser::core

#endif // LASER_CORE_REASONER_H
