//
// Created by mike on 6/15/18.
//

#include "core/program.h"

namespace laser {
namespace core {

Program::Program(rule::RuleReader *rule_reader) {
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

std::vector<std::shared_ptr<util::Grounding>> Program::compute_new_facts() {
    std::vector<std::shared_ptr<util::Grounding>> new_conclusions;
    for (auto const &rule : rule_vector) {
        auto *head = &rule.get_head();
        auto conclusions = head->get_conclusions_step(timeline);
        for (auto &conclusion : conclusions ) {
            new_conclusions.push_back(std::move(conclusion));
        }
    }
    return new_conclusions;
}

std::vector<std::shared_ptr<util::Grounding>> Program::get_conclusions() {
    std::vector<std::shared_ptr<util::Grounding>> new_conclusions;
    for (auto const &rule : rule_vector) {
        auto *head = &rule.get_head();
        auto conclusions = head->get_conclusions_timepoint(timeline);
        for (auto &conclusion : conclusions ) {
            new_conclusions.push_back(std::move(conclusion));
        }
    }
    return new_conclusions;
}

std::vector<std::shared_ptr<util::Grounding>>
Program::evaluate(util::Timeline const &timeline,
                  std::vector<std::shared_ptr<util::Grounding>> data_facts) {
    this->timeline = timeline;
    facts = std::move(data_facts);
    do_evaluation_loop();
    auto conclusions = get_conclusions();
    //! if writing is disabled for benchmarking, this means all tests fail
    return conclusions;
}

} // namespace core 
} // namespace laser
