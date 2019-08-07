#include "acyclicity/naive_smfa.h"

namespace laser::acyclicity {

NaiveSMFA::NaiveSMFA(std::vector<rule::Rule> rule_vector)
    : rule_vector(std::move(rule_vector)) {}

bool NaiveSMFA::has_terminating_chase() {
    compute_extensional_predicates();
    if (extensional_predicates.empty()) {
        // this should be an error
        return true;
    }
    compute_critical_timeline();
    compute_smfa_rule_vector();
    void evaluate_smfa_program();
    return !has_cycle;
}

void NaiveSMFA::compute_extensional_predicates() {
    std::set<std::string> body_set;
    std::set<std::string> head_set;
    for (auto const &rule : rule_vector) {
        auto body_predicates = rule.get_body_predicates();
        body_set.insert(body_predicates.begin(), body_predicates.end());
        auto head_predicates = rule.get_head_predicates();
        head_set.insert(head_predicates.begin(), head_predicates.end());
        auto rule_arity_map = rule.get_arity_map();
        arity_map.insert(rule_arity_map.begin(), rule_arity_map.end());
    }
    for (auto const &body_predicate : body_set) {
        if (head_set.count(body_predicate) == 0) {
            extensional_predicates.insert(body_predicate);
        }
    }
}

std::vector<std::shared_ptr<util::Grounding>>
NaiveSMFA::generate_critical_facts() {
    std::vector<std::shared_ptr<util::Grounding>> result;
    uint64_t max_tuple_counter = laser::util::Timeline::INFINITE_TIME;
    auto time = critical_timeline.get_time();
    for (auto const &predicate : extensional_predicates) {
        std::vector<std::string> constant_vector;
        auto arity = arity_map.at(predicate);
        for (size_t i = 0; i < arity; i++) {
            constant_vector.push_back(special_value::SMFA_VALUE_STAR);
        }
        auto fact = std::make_shared<util::Grounding>(
            predicate, time, time, global_tuple_counter, max_tuple_counter,
            true, false, std::move(constant_vector));
    }
    // all facts have the same tuple counter
    global_tuple_counter += extensional_predicates.size();
    critical_timeline.set_tuple_count(global_tuple_counter);
    return result;
}

void NaiveSMFA::evaluate_smfa_program() {
    auto smfa_program = core::Program(&smfa_rule_vector);
    while (!(has_cycle || critical_timeline.is_past_max_time())) {
        auto critical_facts = generate_critical_facts();
        auto conclusions =
            smfa_program.evaluate(critical_timeline, critical_facts);
        critical_timeline.increment_time();
        for (auto const &conclusion : conclusions) {
            if (conclusion->get_predicate() ==
                special_predicate::SMFA_PREDICATE_C) {
                has_cycle = true;
            }
        }
    }
}

void NaiveSMFA::compute_critical_timeline() {
    // TODO
    critical_timeline.set_max_time(1);
}

void NaiveSMFA::compute_smfa_rule_vector() {
    // TODO
    smfa_rule_vector = rule_vector;
}

} // namespace laser::acyclicity
