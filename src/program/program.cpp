//
// Created by mike on 6/15/18.
//

#include <utility>
#include <rule/default_rule_reader.h>
#include "program/program.h"


namespace laser {
namespace program {

// constructors & destructors

Program::~Program() {
    rule_vector.clear();
}

void Program::evaluate_rule_vector(
        uint64_t current_time,
        uint64_t current_tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        facts) {
    for (auto rule : rule_vector) {
        rule.evaluate(current_time, current_tuple_counter,
                facts);
    }
}

// getters & setters

uint64_t Program::get_current_time() const {
    return current_time;
}

uint64_t Program::get_current_tuple_counter() const {
    return current_tuple_counter;
}

int Program::get_number_of_new_conclusions() const {
    return number_of_new_conclusions;
}

// methods

bool Program::is_done() {
    bool result = false;
    result = has_timeline && current_time >= stream_end_time;
    return result;
}

void Program::expire_outdated_groundings(
        uint64_t current_time,
        uint64_t current_tuple_counter) {
    for (auto rule :rule_vector) {
        rule.expire_outdated_groundings(current_time, current_tuple_counter);
    }
}

bool Program::eval(uint64_t request_time_point) {
    bool has_derived_new_conclusions = false;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            stream_facts
            = ioHandler.get_stream_data(request_time_point);
    expire_outdated_groundings(current_time, current_tuple_counter);
    evaluate_rule_vector(current_time, current_tuple_counter, stream_facts);
    return has_derived_new_conclusions;
}

void Program::write_output(
        std::vector<formula::Formula *>
        new_conclusions) {
    ioHandler.put_conclusions(current_time, std::move(new_conclusions));
}

bool Program::evaluate() {
    uint64_t request_time_point = current_time + 1;
    bool has_derived_new_conclusions = eval(request_time_point);
    if (has_derived_new_conclusions) {
        auto new_conclusions = get_new_conclusions();
        write_output(new_conclusions);
    }
    return has_derived_new_conclusions;
}



void Program::accept_new_facts(
        uint64_t current_time,
        uint64_t current_tuple_counter,
        std::unordered_map<std::string, std::vector<formula::Formula *>>
        stream_facts) {
    for (auto &rule : rule_vector) {
        // TODO rule body accept
    }
}

std::vector<formula::Formula *> Program::get_new_conclusions() {
    // TODO Get new conclusions from each rule as facts of type Formula*
    // TODO combine them in a map
    return std::vector<formula::Formula *>();
}

Program::Program(std::string rule_string, laser::io::IOManager *ioManager) :
        ioManager(ioManager), ioHandler(ioManager) {
    rule::DefaultRuleReader rule_reader(std::move(rule_string));
    rule_vector = rule_reader.get_rules();
}

Program::Program(laser::rule::RuleReader *rule_reader, laser::io::IOManager *ioManager) :
    ioManager(ioManager), ioHandler(ioManager) {
    rule_vector = rule_reader->get_rules();
}


} // namespace program
} // namespace laser
