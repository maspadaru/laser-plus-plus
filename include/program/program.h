//
// Created by mike on 6/15/18.
//
#ifndef LASER_PROGRAM_PROGRAM_H
#define LASER_PROGRAM_PROGRAM_H

#include <any>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <memory>

#include "formula/formula.h"
#include "io/io_manager.h"
#include "io/rule_reader.h"
#include "program/io_handler.h"
#include "rule/default_rule_reader.h"
#include "rule/rule.h"
#include "util/request_exception.h"
#include "util/timeline.h"

namespace laser {
namespace program {

class Program {
  private:
    laser::io::IOManager *ioManager;
    IOHandler ioHandler;

    double evaluation_secs = 0;

    laser::util::Timeline timeline;
    // uint64_t current_time = 0;
    // uint64_t current_tuple_counter = 0;
    // uint64_t stream_start_time = 0;
    // uint64_t stream_end_time = 0;
    int number_of_new_conclusions = 0;
    bool has_timeline = false;

    uint64_t number_of_background_facts = 0;

    std::vector<rule::Rule> rule_vector;

    // methods

    bool evaluate_rule_vector(
        std::unordered_map<
            std::string, std::vector<std::shared_ptr<formula::Grounding>>> const
            &facts);

    bool eval();

    void expire_outdated_groundings();

    // TODO Not used -> remove it
    void accept_new_facts(
        std::unordered_map<std::string, std::vector<std::shared_ptr<formula::Grounding>>> const
            &stream_facts);

    void write_output();

    template <typename T>
    void debug_print(std::string const &message, T const &value) const;

    std::unordered_map<std::string, std::vector<std::shared_ptr<formula::Grounding>>>
    get_new_conclusions();

  public:
    // constructors & destructors

    Program(std::string rule_string, laser::io::IOManager *ioManager);
    Program(laser::rule::RuleReader *rule_reader,
            laser::io::IOManager *ioManager);

    virtual ~Program();

    // getters & setters
    //double get_reader_secs() const;

    //double get_handler_secs() const;

    double get_eval_secs() const;

    uint64_t get_current_time() const;

    uint64_t get_current_tuple_counter() const;

    int get_number_of_new_conclusions() const;

    // methods

    /**
     *
     * @return True if new conclusions were derived from the input facts in
     * the current program time point
     */
    void evaluate();

    bool is_done();

    void set_start_time(uint64_t start_time);
};

} // namespace program
} // namespace laser

#endif // LASER_PROGRAM_PROGRAM_H
