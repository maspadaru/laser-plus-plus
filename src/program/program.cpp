//
// Created by mike on 6/15/18.
//

#include "program/program.h"

namespace laser {
namespace program {

// constructors & destructors

Program::~Program() { rule_vector.clear(); }

bool Program::evaluate_rule_vector(
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>> const
        &facts) {
    bool result = false;
    // Evaluation is done for the previous timepoint
    auto evaluation_timeline = timeline.clone();
    evaluation_timeline.decrement_time();
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
    timeline.set_start_time(start_time);
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

bool Program::do_evaluation_loop(
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>> const
        &initial_facts) {
    bool has_new_conclusions_timepoint = false;
    bool has_new_conclusions_step = false;
    auto facts = initial_facts;
    expire_outdated_groundings();
    do {
        evaluate_rule_vector(facts);
        facts.clear();
        facts = get_new_conclusions();
        has_new_conclusions_step = !facts.empty();
        has_new_conclusions_timepoint |= has_new_conclusions_step;
    } while (has_new_conclusions_step);
    return has_new_conclusions_timepoint;
}

void Program::timed_evaluation(
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>> const
        &facts) {
    clock_t begin = clock();
    auto has_new_conclusions = do_evaluation_loop(facts);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    evaluation_secs += elapsed_secs;
}

std::unordered_map<std::string,
                   std::vector<std::shared_ptr<formula::Grounding>>>
Program::get_new_conclusions() {
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
        new_conclusions;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head();
        auto const &predicate_vector = head->get_predicate_vector();
        for (auto const &conclusion : head->get_conclusions_step(timeline)) {
            // In case head formula has multiple predicates. Might be imposible
            for (auto const &predicate : predicate_vector) {
                new_conclusions.try_emplace(predicate);
                std::vector<std::shared_ptr<formula::Grounding>>
                    &conclusions_vector = new_conclusions[predicate];
                conclusions_vector.push_back(conclusion);
            }
        }
    }
    return new_conclusions;
}

std::unordered_map<std::string,
                   std::vector<std::shared_ptr<formula::Grounding>>>
Program::read_input() {
    if (is_done()) {
        has_input = false;
    } else {
        has_input = true;
        auto facts = ioHandler.get_stream_data(timeline);
    }
}

void Program::write_output() {
    std::vector<formula::Formula *> new_conclusions;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head().clone();
        new_conclusions.push_back(head);
    }
    // Writing is done for the next to last previous timepoint
    auto write_timeline = timeline.clone();
    write_timeline.decrement_time();
    write_timeline.decrement_time();
    ioHandler.put_conclusions(timeline, new_conclusions);
}

void Program::evaluate() {
    // create threads
    auto facts = read_input();
    if (has_input) {
        timed_evaluation(facts);
    }
    if (has_output) {
        // if writing is disabled (e.g.: for benchmarking) all tests will fail
        write_output();
    }
    timeline.increment_time();
    // join threads
}

void Program::accept_new_facts(
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>> const
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

double Program::get_eval_secs() const { return evaluation_secs; }

// double Program::get_reader_secs() const {
// return ioHandler.get_reader_secs();

//}

// double Program::get_handler_secs() const {
// return ioHandler.get_handler_secs();
//}

} // namespace program
} // namespace laser
