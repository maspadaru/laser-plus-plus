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
#include "program/io_handler.h"
#include "rule/default_rule_reader.h"
#include "rule/rule.h"
#include "util/request_exception.h"
#include "util/timeline.h"
#include "util/data_atom.h"

namespace laser {
namespace program {

class Program {
  private:
    IOHandler io_handler;
    std::vector<rule::Rule> rule_vector;
    util::Timeline timeline;
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
        facts;

    void evaluate_rule_vector();

    void do_evaluation_loop();

    void expire_outdated_groundings();

    std::vector<formula::Formula *> get_conclusions();

    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
    compute_new_facts();

  public:

    explicit Program(io::RuleReader *rule_reader);

    virtual ~Program();

    std::vector<util::DataAtom>
        evaluate(util::Timeline const &timeline,
                 std::vector<util::DataAtom> const &data_facts);

    void set_start_time(uint64_t start_time);
};

} // namespace program
} // namespace laser

#endif // LASER_PROGRAM_PROGRAM_H
