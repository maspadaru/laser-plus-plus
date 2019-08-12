#include "core/program.h"

namespace laser::core {

Program::~Program() {
    simple_rule_vector.clear();
    existential_rule_vector.clear();
}

Program::Program(std::vector<rule::Rule> *rule_vector) {
    // auto rule_vector = rule_reader->get_rules();
    sort_rules(rule_vector);
}

void Program::sort_rules(std::vector<rule::Rule> *rule_vector) {
    for (auto &rule : *rule_vector) {
        if (rule.is_existential()) {
            existential_rule_vector.push_back(std::move(rule));
        } else {
            simple_rule_vector.push_back(std::move(rule));
        }
    }
}

void Program::set_start_time(uint64_t start_time) {
    timeline.set_start_time(start_time);
}

void Program::reset_rules() {
    for (auto &rule : simple_rule_vector) {
        rule.reset_previous_step();
        rule.expire_outdated_groundings(timeline);
    }
    for (auto &rule : existential_rule_vector) {
        rule.reset_previous_step();
        rule.expire_outdated_groundings(timeline);
    }
}

void Program::chase_evaluation() {
    reset_rules();
    bool changed = true;
    while (changed) {
        changed = evaluate_rule_vector(simple_rule_vector);
        if (!changed) {
            changed = evaluate_rule_vector(existential_rule_vector);
        }
    }
}

bool Program::evaluate_rule_vector(std::vector<rule::Rule> &rule_vector) {
    bool changed = false;
    for (auto &rule : rule_vector) {
        changed |= evaluate_rule(rule);
    }
    return changed;
}

bool Program::evaluate_rule(rule::Rule &rule) {
    auto step = database.get_step() + 1;
    rule.set_current_step(step);
    rule.evaluate(timeline, database);
    rule.derive_conclusions(timeline, database);
    auto conclusions = rule.get_conclusions_step(timeline);
    bool changed = !conclusions.empty();
    database.increment_step();
    database.insert(step, std::move(conclusions));
    rule.set_previous_step(step);
    return changed;
}

std::vector<std::shared_ptr<util::Grounding>> Program::get_conclusions() {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto &rule : simple_rule_vector) {
        extract_conclusions(rule, result);
    }
    for (auto &rule : existential_rule_vector) {
        extract_conclusions(rule, result);
    }
    return result;
}

void Program::extract_conclusions(
    rule::Rule &rule,
    std::vector<std::shared_ptr<util::Grounding>> &conclusions) const {
    auto rule_conclusions = rule.get_conclusions_timepoint(timeline);
    conclusions.insert(conclusions.end(),
                       make_move_iterator(rule_conclusions.begin()),
                       make_move_iterator(rule_conclusions.end()));
}

std::vector<std::shared_ptr<util::Grounding>>
Program::evaluate(util::Timeline const &timeline,
                  std::vector<std::shared_ptr<util::Grounding>> data_facts) {
    this->timeline = timeline;
    database.clear();
    database.insert_facts(std::move(data_facts));
    chase_evaluation();
    auto conclusions = get_conclusions();
    return conclusions;
}

void Program::init_timepoint(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> data_facts) {
    this->timeline = timeline;
    database.clear();
    database.insert_facts(std::move(data_facts));
    reset_rules();
}

bool Program::evaluate_single_step() {
    bool changed = evaluate_rule_vector(simple_rule_vector);
    changed |= evaluate_rule_vector(existential_rule_vector);
    return changed;
}

std::vector<std::shared_ptr<util::Grounding>>
Program::get_single_step_conclusions() {
    auto conclusions = get_conclusions();
    return conclusions;
}

} // namespace laser::core
