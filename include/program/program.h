//
// Created by mike on 6/15/18.
//
#ifndef LASER_PROGRAM_PROGRAM_H
#define LASER_PROGRAM_PROGRAM_H

#include <vector>
#include <string>
#include <unordered_map>

#include "io/io_manager.h"
#include "io/rule_reader.h"
#include "rule/rule.h"
#include "excetion/request_exception.h"
#include "formula/formula.h"
#include "program/io_handler.h"

namespace laser {
namespace program {

class Program {
private:

    laser::io::IOManager *ioManager;
    IOHandler ioHandler;

    uint64_t current_time = 0;
    uint64_t current_tuple_counter = 0;
    uint64_t stream_start_time = 0;
    uint64_t stream_end_time = 0;
    int number_of_new_conclusions = 0;
    bool has_timeline = false;

    uint64_t number_of_background_facts = 0;

    std::vector<rule::Rule> rule_vector;

// methods

    void evaluate_rule_vector(
            std::unordered_map<std::string, std::vector<formula::Formula *>>
            facts);

    bool eval();

    void expire_outdated_groundings();

    std::vector<formula::Formula *> get_new_conclusions();

    void write_output(
            std::vector<formula::Formula *>
            new_conclusions);


    void accept_new_facts(
            std::unordered_map<std::string, std::vector<formula::Formula *>>
            stream_facts);


public:

// constructors & destructors

    Program(std::string rule_string, laser::io::IOManager *ioManager);
    Program(laser::rule::RuleReader *rule_reader, laser::io::IOManager *ioManager);

    virtual ~Program();

// getters & setters

    uint64_t get_current_time() const;

    uint64_t get_current_tuple_counter() const;

    int get_number_of_new_conclusions() const;

// methods

    /**
     *
     * @return True if new conclusions were derived from the input facts in
     * the current program time point
     */
    bool evaluate();

    bool is_done();

    void set_start_time(uint64_t start_time);

};

} // namespace program
} // namespace laser



#endif // LASER_PROGRAM_PROGRAM_H
