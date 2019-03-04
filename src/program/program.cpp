//
// Created by mike on 6/15/18.
//

#include "program/program.h"

namespace laser {
namespace program {


Program::Program(io::RuleReader *rule_reader) {
    rule_vector = rule_reader->get_rules();
}

Program::~Program() { rule_vector.clear(); }

void Program::evaluate_rule_vector() {
    for (auto &rule : rule_vector) {
        rule.evaluate(timeline, facts);
        rule.derive_conclusions(timeline);
    }
}

void Program::set_start_time(uint64_t start_time) {
    timeline.set_start_time(start_time);
}

void Program::expire_outdated_groundings() {
    for (auto &rule : rule_vector) {
        rule.expire_outdated_groundings(timeline);
    }
}

void Program::do_evaluation_loop() {
    bool has_new_conclusions = false;
    expire_outdated_groundings();
    do {
        evaluate_rule_vector();
        facts.clear();
        facts = compute_new_facts();
        has_new_conclusions = !facts.empty();
    } while (has_new_conclusions);
}

std::unordered_map<std::string,
                   std::vector<std::shared_ptr<formula::Grounding>>>
Program::compute_new_facts() {
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
        new_conclusions;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head();
        auto const &predicate_vector = head->get_predicate_vector();
        for (auto &conclusion : head->get_conclusions_step(timeline)) {
            // In case head formula has multiple predicates. Might be imposible
            for (auto const &predicate : predicate_vector) {
                new_conclusions.try_emplace(predicate);
                std::vector<std::shared_ptr<formula::Grounding>>
                    &conclusions_vector = new_conclusions[predicate];
                conclusions_vector.push_back(std::move(conclusion));
            }
        }
    }
    return new_conclusions;
}

std::vector<formula::Formula *> Program::get_conclusions() {
    std::vector<formula::Formula *> result;
    for (auto const &rule : rule_vector) {
        formula::Formula *head = &rule.get_head();
        result.push_back(head);
    }
    return result;
}

std::vector<util::DataAtom>
Program::evaluate(util::Timeline const &timeline,
                  std::vector<util::DataAtom> const &data_facts) {
    this->timeline = timeline;
    facts = io_handler.handle_input(this->timeline, data_facts);
    do_evaluation_loop();
    auto conclusions = get_conclusions();
    //! if writing is disabled for benchmarking, this means all tests fail
    auto result = io_handler.handle_output(timeline, conclusions);
    return result;
}

} // namespace program
} // namespace laser
