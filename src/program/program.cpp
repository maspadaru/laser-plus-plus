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
        rule.evaluate(timeline, facts);
        result |= rule.derive_conclusions(timeline);
    }
    return result;
}

// getters & setters

uint64_t Program::get_current_time() const { return timeline.get_time(); }

uint64_t Program::get_current_tuple_counter() const {
    return timeline.get_tuple_count();
}

int Program::get_number_of_new_conclusions() const {
    return number_of_new_conclusions;
}

// methods

template <typename T>
void Program::debug_print(std::string const &message, T const &value) const {
    std::cerr << "Program -> current time: " << timeline.get_time() << " -> ";
    std::cerr << message << " : " << value << std::endl;
}

void Program::set_start_time(uint64_t start_time) {
    timeline.set_min_time(start_time);
}

bool Program::is_done() {
    if (!has_timeline) {
        timeline.set_min_time(ioManager->read_stream_start_time());
        timeline.set_max_time(ioManager->read_stream_end_time());
        has_timeline = true;
    }
    bool keep_going = has_timeline && !timeline.is_past_max_time();
    return !keep_going;
}

void Program::expire_outdated_groundings() {
    for (auto &rule : rule_vector) {
        rule.expire_outdated_groundings(timeline);
    }
}

bool Program::eval() {
    bool has_derived_new_conclusions = false;
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts =
        ioHandler.get_stream_data(timeline.get_time());
    expire_outdated_groundings();
    while (!facts.empty()) {
        evaluate_rule_vector(facts);
        facts.clear();
        facts = get_new_conclusions();
        has_derived_new_conclusions |= !facts.empty(); 
    }
    return has_derived_new_conclusions;
}

std::unordered_map<std::string, std::vector<formula::Grounding>>
Program::get_new_conclusions() {
    std::unordered_map<std::string, std::vector<formula::Grounding>> new_conclusions;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head();
        auto const &predicate_vector = head->get_predicate_vector();
        for (auto const &conclusion : head->get_conclusions(timeline)) {
            // In case head formula has multiple predicates. Might be imposible
            for (auto const &predicate : predicate_vector) {
                new_conclusions.try_emplace(predicate);
                std::vector<formula::Grounding> &conclusions_vector = new_conclusions[predicate];
                conclusions_vector.push_back(conclusion);
            }
        }
    }
    return new_conclusions;
}

void Program::write_output() {
    std::vector<formula::Formula *> new_conclusions;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head();
        new_conclusions.push_back(head);
    }
    ioHandler.put_conclusions(timeline, new_conclusions);
}

void Program::evaluate() {
    bool has_derived_new_conclusions = eval();
    if (has_derived_new_conclusions) {
        write_output();
    }
    timeline.increment_time();
}

void Program::accept_new_facts(
    std::unordered_map<std::string, std::vector<formula::Grounding>> const
        &stream_facts) {
    for (auto &rule : rule_vector) {
        rule.evaluate(timeline, stream_facts);
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
