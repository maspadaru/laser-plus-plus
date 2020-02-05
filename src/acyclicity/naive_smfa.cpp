#include "acyclicity/naive_smfa.h"

namespace laser::acyclicity {

NaiveSMFA::NaiveSMFA(
    std::vector<std::unique_ptr<rule::Rule>> const &input_program) {
    for (auto const &rule : input_program) {
        rule_vector.push_back(rule->clone());
    }
}

bool NaiveSMFA::has_terminating_chase() {
    compute_extensional_predicates();
    if (extensional_predicates.empty()) {
        // this should be an error
        return true;
    }
    compute_critical_timeline();
    compute_smfa_rule_vector();
    evaluate_smfa_program();
    return !has_cycle;
}

void NaiveSMFA::compute_extensional_predicates() {
    std::set<std::string> body_set;
    std::set<std::string> head_set;
    for (auto const &rule : rule_vector) {
        auto body_predicates = rule->get_body_predicates();
        body_set.insert(body_predicates.begin(), body_predicates.end());
        auto head_predicates = rule->get_head_predicates();
        head_set.insert(head_predicates.begin(), head_predicates.end());
        auto rule_arity_map = rule->get_arity_map();
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
        result.push_back(fact);
    }
    // all facts have the same tuple counter
    global_tuple_counter += extensional_predicates.size();
    critical_timeline.set_tuple_count(global_tuple_counter);
    return result;
}

void NaiveSMFA::evaluate_smfa_program() {
    auto smfa_program = core::Program(smfa_rule_vector);
    while (!(has_cycle || critical_timeline.is_past_max_time())) {
        auto critical_facts = generate_critical_facts();
        smfa_program.init_timepoint(critical_timeline, critical_facts);
        bool done = false;
        while (!done) {
            done = !smfa_program.evaluate_single_step();
            auto conclusions = smfa_program.get_single_step_conclusions();
            for (auto const &conclusion : conclusions) {
                if (conclusion->get_predicate() ==
                    special_predicate::SMFA_PREDICATE_C) {
                    has_cycle = true;
                    done = true;
                }
            }
        }
        critical_timeline.increment_time();
    }
}

uint64_t NaiveSMFA::get_time(std::unique_ptr<formula::Formula> const &formula,
                             bool is_head_atom) {
    uint64_t result = 0;
    auto formula_type = formula->get_type();
    if (formula_type == formula::FormulaType::ATOM) {
        return 0;
    }
    switch (formula_type) {
    case formula::FormulaType::CONJUNCTION: {
        auto children = formula->get_children();
        for (auto const *child : children) {
            result += get_time(*child, is_head_atom);
        }
        break;
    }
    case formula::FormulaType::TIME_REFERENCE: {
        if (is_head_atom) {
            result = 0;
            // TODO: not supported by Laser++ a.t.m.
            // TODO need to implement MATH atoms in Laser++

        } else {
            auto const *child = formula->get_children().at(0);
            result = get_time(*child, is_head_atom);
        }
        break;
    }
    case formula::FormulaType::DIAMOND: {
        auto const *child = formula->get_children().at(0);
        result = get_time(*child, is_head_atom);
        break;
    }
    case formula::FormulaType::BOX: {
        result = util::Timeline::INFINITE_TIME;
        break;
    }
    case formula::FormulaType::TIME_WINDOW: {
        auto window_size = formula->get_window_size();
        auto const *child = formula->get_children().at(0);
        uint64_t child_time = get_time(*child, is_head_atom);
        result = std::min(window_size, child_time);
        break;
    }
    case formula::FormulaType::TUPLE_WINDOW: {
        auto tuple_size = formula->get_window_size();
        auto const *child = formula->get_children().at(0);
        uint64_t child_time = get_time(*child, is_head_atom);
        auto extensional_predicate_count = extensional_predicates.size();
        double float_time_size =
            (tuple_size * 1.0) / extensional_predicate_count;
        uint64_t time_size = std::ceil(float_time_size);
        result = std::min(time_size, child_time);
        break;
    }
    default: { break; }
    }
    return result;
}

void NaiveSMFA::compute_critical_timeline() {
    uint64_t result = 1;
    for (auto const &rule : rule_vector) {
        auto const &head_atoms = rule->get_head_atoms();
        auto const &body = rule->get_body();
        result += get_time(body, false);
        for (auto const &atom : head_atoms) {
            result += get_time(atom, true);
        }
    }
    critical_timeline.set_max_time(result);
}

std::string NaiveSMFA::generate_new_F_predicate(size_t rule_index,
                                                size_t bound_index) const {
    std::string result = acyclicity::special_predicate::SMFA_PREDICATE_F +
                         " _rule_" + std::to_string(rule_index) +
                         "_bound_variable_" + std::to_string(bound_index);
    return result;
}

std::unique_ptr<formula::Formula>
NaiveSMFA::generate_new_unary_atom(std::string const &predicate,
                                   std::string const &variable) const {
    std::vector<std::string> variable_vector;
    variable_vector.push_back(variable);
    auto result = std::make_unique<formula::Atom>(predicate, variable_vector);
    return result;
}

std::unique_ptr<formula::Formula>
NaiveSMFA::generate_new_binary_atom(std::string const &predicate,
                                    std::string const &first_variable,
                                    std::string const &second_variable) const {
    std::vector<std::string> variable_vector;
    variable_vector.push_back(first_variable);
    variable_vector.push_back(second_variable);
    auto result = std::make_unique<formula::Atom>(predicate, variable_vector);
    return result;
}

std::unique_ptr<rule::Rule> NaiveSMFA::generate_S_D_rule() const {
    // D(X, Y) := S(X, Y)
    std::string variable_X = "X";
    std::string variable_Y = "Y";
    std::string predicate_D = acyclicity::special_predicate::SMFA_PREDICATE_D;
    std::string predicate_S = acyclicity::special_predicate::SMFA_PREDICATE_S;
    auto atom_D = generate_new_binary_atom(predicate_D, variable_X, variable_Y);
    auto atom_S = generate_new_binary_atom(predicate_S, variable_X, variable_Y);
    std::vector<std::unique_ptr<formula::Formula>> head_atoms;
    std::vector<std::unique_ptr<formula::Formula>> body_atoms;
    head_atoms.push_back(std::move(atom_D));
    body_atoms.push_back(std::move(atom_S));
    return std::make_unique<rule::Rule>(std::move(body_atoms),
                                        std::move(head_atoms));
}

std::unique_ptr<rule::Rule> NaiveSMFA::generate_D_transitive_rule() const {
    // D(X, Z) := D(X, Y) && S(Y, Z)
    std::string variable_X = "X";
    std::string variable_Y = "Y";
    std::string variable_Z = "Z";
    std::string predicate_D = acyclicity::special_predicate::SMFA_PREDICATE_D;
    std::string predicate_S = acyclicity::special_predicate::SMFA_PREDICATE_S;
    auto head_atom_D =
        generate_new_binary_atom(predicate_D, variable_X, variable_Z);
    auto body_atom_D =
        generate_new_binary_atom(predicate_D, variable_X, variable_Y);
    auto body_atom_S =
        generate_new_binary_atom(predicate_S, variable_Y, variable_Z);
    std::vector<std::unique_ptr<formula::Formula>> head_atoms;
    std::vector<std::unique_ptr<formula::Formula>> body_atoms;
    head_atoms.push_back(std::move(head_atom_D));
    body_atoms.push_back(std::move(body_atom_D));
    body_atoms.push_back(std::move(body_atom_S));
    return std::make_unique<rule::Rule>(std::move(body_atoms), std::move(head_atoms));
}

std::unique_ptr<rule::Rule>
NaiveSMFA::generate_C_rule(std::string const &predicate_F) const {
    // C := F(X) && D(X, Y) && F(Y)
    std::string variable_X = "X";
    std::string variable_Y = "Y";
    std::string predicate_D = acyclicity::special_predicate::SMFA_PREDICATE_D;
    std::string predicate_C = acyclicity::special_predicate::SMFA_PREDICATE_C;
    // auto head_atom_C = new formula::Atom(predicate_C);
    auto head_atom_C =
        generate_new_binary_atom(predicate_C, variable_X, variable_Y);
    auto body_atom_D =
        generate_new_binary_atom(predicate_D, variable_X, variable_Y);
    auto body_atom_F_X = generate_new_unary_atom(predicate_F, variable_X);
    auto body_atom_F_Y = generate_new_unary_atom(predicate_F, variable_Y);
    std::vector<std::unique_ptr<formula::Formula>> head_atoms;
    std::vector<std::unique_ptr<formula::Formula>> body_atoms;
    head_atoms.push_back(std::move(head_atom_C));
    body_atoms.push_back(std::move(body_atom_D));
    body_atoms.push_back(std::move(body_atom_F_X));
    body_atoms.push_back(std::move(body_atom_F_Y));
    return std::make_unique<rule::Rule>(std::move(body_atoms), std::move(head_atoms));
}

void NaiveSMFA::compute_smfa_rule_vector() {
    std::vector<std::string> f_predicate_vector;
    for (auto const &rule : rule_vector) {
        smfa_rule_vector.push_back(rule->clone());
    }
    for (size_t rule_index = 0; rule_index < smfa_rule_vector.size();
         rule_index++) {
        auto &rule = smfa_rule_vector.at(rule_index);
        if (rule->is_existential()) {
            std::vector<std::unique_ptr<formula::Formula>> new_atom_vector;
            std::vector<std::string> bound_list = rule->get_bound_variables();
            std::vector<std::string> frontier_list =
                rule->get_frontier_variables();
            for (size_t bound_index = 0; bound_index < bound_list.size();
                 bound_index++) {
                auto bound_variable = bound_list.at(bound_index);
                auto f_predicate =
                    generate_new_F_predicate(rule_index, bound_index);
                f_predicate_vector.push_back(f_predicate);
                auto f_atom =
                    generate_new_unary_atom(f_predicate, bound_variable);
                new_atom_vector.push_back(std::move(f_atom));
                for (auto const &frontier_variable : frontier_list) {
                    auto s_predicate =
                        acyclicity::special_predicate::SMFA_PREDICATE_S;
                    auto s_atom = generate_new_binary_atom(
                        s_predicate, frontier_variable, bound_variable);
                    new_atom_vector.push_back(std::move(s_atom));
                }
            }
            rule->add_head_atoms(std::move(new_atom_vector));
        }
    }
    auto s_d_rule = generate_S_D_rule();
    auto d_transitive_rule = generate_D_transitive_rule();
    smfa_rule_vector.push_back(std::move(s_d_rule));
    smfa_rule_vector.push_back(std::move(d_transitive_rule));
    for (auto const &f_predicate : f_predicate_vector) {
        auto c_rule = generate_C_rule(f_predicate);
        smfa_rule_vector.push_back(std::move(c_rule));
    }
}

} // namespace laser::acyclicity
