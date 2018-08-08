//
// Created by mike on 6/15/18.
//

#include "program/program.h"

namespace laser {
namespace program {

// constructors & destructors

Program::~Program() { rule_vector.clear(); }

bool Program::evaluate_rule_vector(
    std::unordered_map<std::string, std::vector<formula::Grounding>> const
        &facts) {
    bool result = false;
    for (auto &rule : rule_vector) {
        rule.evaluate(current_time, current_tuple_counter, facts);
        result |= rule.derive_conclusions(current_time, current_tuple_counter);
    }
    return result;
}

// getters & setters

uint64_t Program::get_current_time() const { return current_time; }

uint64_t Program::get_current_tuple_counter() const {
    return current_tuple_counter;
}

int Program::get_number_of_new_conclusions() const {
    return number_of_new_conclusions;
}

// methods

template <typename T>
void Program::debug_print(std::string const &message, T const &value) const {
    std::cerr << "Program -> current time: " << current_time << " -> ";
    std::cerr << message << " : " << value << std::endl;
}

void Program::set_start_time(uint64_t start_time) { current_time = start_time; }

bool Program::is_done() {
    if (!has_timeline) {
        stream_start_time = ioManager->read_stream_start_time();
        stream_end_time = ioManager->read_stream_end_time();
        has_timeline = true;
        debug_print("start time", stream_start_time);
        debug_print("end time", stream_end_time);
    }
    bool keep_going = has_timeline && current_time <= stream_end_time;
    debug_print("keep going:", keep_going);
    return !keep_going;
}

void Program::expire_outdated_groundings() {
    for (auto &rule : rule_vector) {
        rule.expire_outdated_groundings(current_time, current_tuple_counter);
    }
}

bool Program::eval() {
    std::unordered_map<std::string, std::vector<formula::Grounding>>
        stream_facts = ioHandler.get_stream_data(current_time);
    expire_outdated_groundings();
    bool has_derived_new_conclusions = evaluate_rule_vector(stream_facts);
    return has_derived_new_conclusions;
}

void Program::write_output() {
    std::vector<formula::Formula *> new_conclusions;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head();
        debug_print("RuleHead as onclusion: ", head->debug_string());
        new_conclusions.push_back(head);
    }
    ioHandler.put_conclusions(current_time, new_conclusions);
}

void Program::evaluate() {
    bool has_derived_new_conclusions = eval();
    if (has_derived_new_conclusions) {
        write_output();
    }
    current_time++;
}

void Program::accept_new_facts(
    std::unordered_map<std::string, std::vector<formula::Grounding>> const
        &stream_facts) {
    for (auto &rule : rule_vector) {
        rule.evaluate(current_time, current_tuple_counter, stream_facts);
    }
}

Program::Program(std::string rule_string, laser::io::IOManager *ioManager)
    : ioManager(ioManager), ioHandler(ioManager) {
    rule::DefaultRuleReader rule_reader(std::move(rule_string));
    rule_vector = rule_reader.get_rules();
}

Program::Program(laser::rule::RuleReader *rule_reader,
                 laser::io::IOManager *ioManager)
    : ioManager(ioManager), ioHandler(ioManager) {
    rule_vector = rule_reader->get_rules();
}

} // namespace program
} // namespace laser
