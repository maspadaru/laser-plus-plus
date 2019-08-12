#ifndef LASER_CORE_PROGRAM_H
#define LASER_CORE_PROGRAM_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "rule/rule.h"
#include "util/database.h"
#include "util/timeline.h"

namespace laser::core {

class Program {
  private:
    std::vector<rule::Rule> simple_rule_vector;
    std::vector<rule::Rule> existential_rule_vector;
    util::Timeline timeline;
    util::Database database;

    void chase_evaluation();
    void sort_rules(std::vector<rule::Rule> *rule_vector);
    void reset_rules();
    bool evaluate_rule_vector(std::vector<rule::Rule> &rule_vector);
    bool evaluate_rule(rule::Rule &rule);
    std::vector<std::shared_ptr<util::Grounding>> get_conclusions();
    void extract_conclusions(
        rule::Rule &rule,
        std::vector<std::shared_ptr<util::Grounding>> &conclusions) const;

    // void do_evaluation_loop();
    // std::vector<std::shared_ptr<util::Grounding>> compute_new_facts();

  public:
    explicit Program(std::vector<rule::Rule> *rule_vector);

    virtual ~Program();

    std::vector<std::shared_ptr<util::Grounding>>
    evaluate(util::Timeline const &timeline,
             std::vector<std::shared_ptr<util::Grounding>> data_facts);

    void set_start_time(uint64_t start_time);

    void
    init_timepoint(util::Timeline const &timeline,
                   std::vector<std::shared_ptr<util::Grounding>> data_facts);

    bool evaluate_single_step();

    std::vector<std::shared_ptr<util::Grounding>> get_single_step_conclusions();
};

} // namespace laser::core

#endif // LASER_CORE_PROGRAM_H
