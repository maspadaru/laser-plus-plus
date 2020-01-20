#include "core/program.h"

namespace laser::core {

Program::~Program() {
    simple_rule_vector.clear();
    existential_rule_vector.clear();
}

Program::Program(std::vector<std::unique_ptr<laser::rule::Rule>> &rule_vector) {
    // auto rule_vector = rule_reader->get_rules();
    sort_rules(rule_vector);
}

void Program::sort_rules(
    std::vector<std::unique_ptr<laser::rule::Rule>> &rule_vector) {
    for (auto &rule : rule_vector) {
        if (rule->is_existential()) {
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
        rule->reset_previous_step();
        rule->expire_outdated_groundings(timeline);
    }
    for (auto &rule : existential_rule_vector) {
        rule->reset_previous_step();
        rule->expire_outdated_groundings(timeline);
    }
}

void Program::chase_evaluation() {
    reset_rules();
    // facts derived at previous time-points that are still valid now.
    auto initial_conclusions = get_conclusions();
    //auto step = database.get_step();
    //database.insert(step, std::move(initial_conclusions));
    database.insert_in_predicate_map(initial_conclusions);
    database.update_predicate_map();
    // chase:
    bool changed = true;
    while (changed) {
        auto step = database.get_step() + 1;
        changed = evaluate_rule_vector(simple_rule_vector, step);
        if (!changed) {
            changed = evaluate_rule_vector(existential_rule_vector, step);
        }
        database.increment_step();
        database.insert(step, std::move(new_conclusions));
        database.update_predicate_map();
    }
}

bool Program::evaluate_rule_vector(
    std::vector<std::unique_ptr<laser::rule::Rule>> &rule_vector, size_t step) {
    bool changed = false;
    for (auto &rule : rule_vector) {
        changed |= evaluate_rule(*rule, step);
    }
    return changed;
}

bool Program::evaluate_rule(rule::Rule &rule, size_t step) {
    rule.set_current_step(step);
    rule.evaluate(timeline, database);
    rule.derive_conclusions(timeline, database);
    auto conclusions = rule.get_conclusions_step(timeline);
    database.insert_in_predicate_map(conclusions);
    bool changed = !conclusions.empty();
    new_conclusions.insert(new_conclusions.end(),
                           std::make_move_iterator(conclusions.begin()),
                           std::make_move_iterator(conclusions.end()));
    rule.set_previous_step(step);
    return changed;
}

std::vector<std::shared_ptr<util::Grounding>> Program::get_conclusions() {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto &rule : simple_rule_vector) {
        extract_conclusions(*rule, result);
    }
    for (auto &rule : existential_rule_vector) {
        extract_conclusions(*rule, result);
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
    auto clock_start = std::chrono::high_resolution_clock::now();
    database.insert_facts(std::move(data_facts));
    chase_evaluation();
    auto conclusions = get_conclusions();
    auto clock_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> clock_elapsed =
        clock_end - clock_start;
    double elapsed_seconds = clock_elapsed.count() / 1000;
    runtimes.push_back(elapsed_seconds);
    //std::cout << "Input size: " << database.get_data_step(0).size() << std::endl;
    //std::cout << "Number of Steps: " << database.get_step() << std::endl;
    //std::cout << "Total Conclusions: " << conclusions.size() << std::endl;
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
    auto step = database.get_step() + 1;
    bool changed = evaluate_rule_vector(simple_rule_vector, step);
    changed |= evaluate_rule_vector(existential_rule_vector, step);
    database.increment_step();
    database.insert(step, std::move(new_conclusions));
    return changed;
}

std::vector<std::shared_ptr<util::Grounding>>
Program::get_single_step_conclusions() {
    auto conclusions = get_conclusions();
    return conclusions;
}


std::vector<double> const &Program::get_runtimes() const {
    return runtimes;
}

} // namespace laser::core
