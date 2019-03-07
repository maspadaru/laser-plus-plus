//
// Created by mike on 6/15/18.
//
#ifndef LASER_CORE_PROGRAM_H
#define LASER_CORE_PROGRAM_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "rule/rule_reader.h"
#include "rule/rule.h"
#include "util/timeline.h"

namespace laser {
namespace core {

class Program {
  private:
    std::vector<rule::Rule> rule_vector;
    util::Timeline timeline;
    std::vector<std::shared_ptr<util::Grounding>> facts;

    void evaluate_rule_vector();

    void do_evaluation_loop();

    void expire_outdated_groundings();

    std::vector<std::shared_ptr<util::Grounding>> get_conclusions();

    std::vector<std::shared_ptr<util::Grounding>> compute_new_facts();

  public:
    explicit Program(rule::RuleReader *rule_reader);

    virtual ~Program();

    std::vector<std::shared_ptr<util::Grounding>>
    evaluate(util::Timeline const &timeline,
             std::vector<std::shared_ptr<util::Grounding>> data_facts);

    void set_start_time(uint64_t start_time);
};

} // namespace core 
} // namespace laser

#endif // LASER_CORE_PROGRAM_H
