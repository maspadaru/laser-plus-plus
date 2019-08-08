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

uint64_t NaiveSMFA::get_time(formula::Formula *formula, bool is_head_atom) {
    uint64_t result = 0;
    auto formula_type = formula->get_type();
    if (formula_type == formula::FormulaType::ATOM) {
        return 0;
    }
    switch (formula_type) {
    case formula::FormulaType::CONJUNCTION: {
        auto children = formula->get_children();
        for (auto *child : children) {
            result += get_time(child, is_head_atom);
        }
        break;
    }
    case formula::FormulaType::TIME_REFERENCE: {
        if (is_head_atom) {
            result = 0;
            // TODO: not supported by Laser++ a.t.m.
            // TODO need to implement MATH atoms in Laser++

        } else {
            auto child = formula->get_children().at(0);
            result = get_time(child, is_head_atom);
        }
        break;
    }
    case formula::FormulaType::DIAMOND: {
        auto child = formula->get_children().at(0);
        result = get_time(child, is_head_atom);
        break;
    }
    case formula::FormulaType::BOX: {
        result = util::Timeline::INFINITE_TIME;
        break;
    }
    case formula::FormulaType::TIME_WINDOW: {
        auto window_size = formula->get_window_size();
        auto child = formula->get_children().at(0);
        uint64_t child_time = get_time(child, is_head_atom);
        result = std::min(window_size, child_time);
        break;
    }
    case formula::FormulaType::TUPLE_WINDOW: {
        auto tuple_size = formula->get_window_size();
        auto child = formula->get_children().at(0);
        uint64_t child_time = get_time(child, is_head_atom);
        auto extensional_predicate_count = extensional_predicates.size();
        double float_time_size = tuple_size / extensional_predicate_count;
        uint64_t time_size = std::ceil(float_time_size);
        result = std::min(time_size, child_time);
        break;
    }
    }
    return result;
}

void NaiveSMFA::compute_critical_timeline() {
    uint64_t sum = 1;
    for (auto const &rule : rule_vector) {
        auto head_atoms = rule.get_head_atoms();
        auto *body = rule.get_body();
        sum += get_time(body, false);
        for (auto *atom : head_atoms) {
            sum += get_time(atom, true);
        }
    }
    critical_timeline.set_max_time(1);
}

void NaiveSMFA::compute_smfa_rule_vector() {
    // TODO
    smfa_rule_vector = rule_vector;
}

} // namespace laser::acyclicity
