//
// Created by mike on 6/15/18.
//
#ifndef LASER_PROGRAM_PROGRAM_H
#define LASER_PROGRAM_PROGRAM_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "formula/formula.h"
#include "io/rule_reader.h"
#include "rule/default_rule_reader.h"
#include "rule/rule.h"
#include "util/request_exception.h"
#include "util/timeline.h"

namespace laser {
namespace program {

class Program {
  private:
    std::vector<rule::Rule> rule_vector;
    util::Timeline timeline;
    std::vector<std::shared_ptr<formula::Grounding>> facts;

    void evaluate_rule_vector();

    void do_evaluation_loop();

    void expire_outdated_groundings();

    std::vector<std::shared_ptr<formula::Grounding>> get_conclusions();

    std::vector<std::shared_ptr<formula::Grounding>> compute_new_facts();

  public:
    explicit Program(io::RuleReader *rule_reader);

    virtual ~Program();

    std::vector<std::shared_ptr<formula::Grounding>>
    evaluate(util::Timeline const &timeline,
             std::vector<std::shared_ptr<formula::Grounding>> data_facts);

    void set_start_time(uint64_t start_time);
};

} // namespace program
} // namespace laser

#endif // LASER_PROGRAM_PROGRAM_H
