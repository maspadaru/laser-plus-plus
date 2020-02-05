#include "rule/rule.h"

namespace laser::rule {

Rule::Rule(std::vector<std::unique_ptr<formula::Formula>> body_atoms,
           std::vector<std::unique_ptr<formula::Formula>> head_atoms) {
    init(std::move(body_atoms), std::move(head_atoms));
}

Rule::Rule(std::vector<std::unique_ptr<formula::Formula>> body_atoms,
           std::vector<std::unique_ptr<formula::Formula>> head_atoms,
           std::set<std::string> event_variables)
    : event_variables(std::move(event_variables)) {
    init(std::move(body_atoms), std::move(head_atoms));
}

Rule::Rule(std::unique_ptr<formula::Formula> body_formula,
           std::vector<std::unique_ptr<formula::Formula>> head_atoms,
           std::set<std::string> event_variables)
    : body(std::move(body_formula)),
      event_variables(std::move(event_variables)) {
    init_head(std::move(head_atoms));
}

std::unique_ptr<Rule> Rule::clone() const {
    auto body_clone = body->clone();
    std::vector<std::unique_ptr<formula::Formula>> clone_head_atoms;
    for (auto const &atom : head_atoms) {
        clone_head_atoms.push_back(atom->clone());
    }
    return std::make_unique<rule::Rule>(
        std::move(body_clone), std::move(clone_head_atoms), event_variables);
}

bool Rule::is_existential() const { return is_existential_m; }

std::vector<std::unique_ptr<formula::Formula>> const &
Rule::get_head_atoms() const {
    return head_atoms;
}

void Rule::add_head_atoms(
    std::vector<std::unique_ptr<formula::Formula>> atom_vector) {
    head_atoms.insert(head_atoms.end(),
                      std::make_move_iterator(atom_vector.begin()),
                      std::make_move_iterator(atom_vector.end()));
    init_head(std::move(head_atoms));
}

std::unique_ptr<formula::Formula> const &Rule::get_body() const { return body; }

std::set<std::string> Rule::get_body_predicates() const {
    std::set<std::string> result;
    auto vector = body->get_predicate_vector();
    std::copy(vector.begin(), vector.end(),
              std::inserter(result, result.end()));
    return result;
}

std::set<std::string> Rule::get_head_predicates() const {
    std::set<std::string> result;
    for (auto const &atom : head_atoms) {
        auto vector = atom->get_predicate_vector();
        std::copy(vector.begin(), vector.end(),
                  std::inserter(result, result.end()));
    }
    return result;
}

std::map<std::string, size_t> Rule::get_arity_map() const {
    std::map<std::string, size_t> result;
    auto body_arity_map = body->get_arity_map();
    result.insert(body_arity_map.begin(), body_arity_map.end());
    for (auto const &atom : head_atoms) {
        auto head_arity_map = atom->get_arity_map();
        result.insert(head_arity_map.begin(), head_arity_map.end());
    }
    return result;
}

void Rule::reset_previous_step() {
    previous_step = 0;
    current_step = 0;
}

void Rule::set_previous_step(size_t step) { previous_step = step; }

void Rule::set_current_step(size_t step) { current_step = step; }

std::vector<std::shared_ptr<util::Grounding>>
Rule::get_conclusions_step(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &head_atom : head_atoms) {
        auto head_groundings = head_atom->get_conclusions_step(timeline);
        result.insert(result.end(), head_groundings.begin(),
                      head_groundings.end());
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Rule::get_conclusions_timepoint(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &head_atom : head_atoms) {
        auto head_groundings = head_atom->get_conclusions_timepoint(timeline);
        result.insert(result.end(), head_groundings.begin(),
                      head_groundings.end());
    }
    return result;
}

void Rule::expire_outdated_groundings(util::Timeline const &timeline) {
    expire_head_groundings(timeline);
    body->expire_outdated_groundings(timeline);
    chase_filter->expire_outdated_groundings(timeline);
}

void Rule::expire_head_groundings(util::Timeline const &timeline) {
    for (auto const &atom : head_atoms) {
        atom->expire_outdated_groundings(timeline);
    }
}

void Rule::evaluate(util::Timeline const &timeline,
                    util::Database const &database) {
    auto facts = database.get_data_since(previous_step);
    body->evaluate(timeline, previous_step, facts);
}

void Rule::init_frontier_variables(
    std::vector<std::unique_ptr<formula::Formula>> const &head_atoms) {
    // I am igniring the time variable here, else I will always get new values
    // at each timepoint
    std::set<std::string> atom_variable_set;
    for (auto const &atom : head_atoms) {
        auto variable_names = atom->get_variable_names();
        if (atom->get_type() == formula::FormulaType::TIME_REFERENCE) {
            // Time variable is always the last
            std::copy(
                variable_names.begin(), variable_names.end() - 1,
                std::inserter(atom_variable_set, atom_variable_set.end()));
        } else {
            std::copy(
                variable_names.begin(), variable_names.end(),
                std::inserter(atom_variable_set, atom_variable_set.end()));
        }
    }

    for (auto const &variable : bound_variables) {
        atom_variable_set.erase(variable);
    }
    std::copy(atom_variable_set.begin(), atom_variable_set.end(),
              std::back_inserter(frontier_variables));
}

void Rule::init_chase(
    std::vector<std::unique_ptr<formula::Formula>> const &head_atoms) {
    std::vector<std::string> free_variables = body->get_variable_names();
    std::set<std::string> free_variable_set(free_variables.begin(),
                                            free_variables.end());
    std::vector<std::string> head_variables;
    head_variables = free_variables;
    std::set<std::string> bound_variable_set;
    for (auto const &atom : head_atoms) {
        auto const &variable_names = atom->get_variable_names();
        for (auto const &var_name : variable_names) {
            if (free_variable_set.count(var_name) == 0) {
                bound_variable_set.insert(var_name);
            }
        }
    }
    if (bound_variable_set.empty()) {
        is_existential_m = false;
        chase_filter = std::make_unique<ObliviousFilter>();
    } else {
        is_existential_m = true;
        std::copy(bound_variable_set.begin(), bound_variable_set.end(),
                  std::back_inserter(bound_variables));
        head_variables.insert(head_variables.end(), bound_variables.begin(),
                              bound_variables.end());
        init_frontier_variables(head_atoms);
        auto chase_algorithm =
            util::Settings::get_instance().get_chase_algorithm();
        switch (chase_algorithm) {
        case util::ChaseAlgorithm::RESTRICTED:
            chase_filter = std::make_unique<RestrictedFilter>();
            break;
        case util::ChaseAlgorithm::SKOLEM:
            chase_filter = std::make_unique<SkolemFilter>();
            break;
        default:
            chase_filter = std::make_unique<ObliviousFilter>();
            break;
        }
        std::vector<bool> is_event_variable;
        if (!event_variables.empty()) {
            is_event_variable = generate_event_vector();
        }
        chase_filter->init(head_atoms, head_variables, free_variables,
                           bound_variables, is_event_variable,
                           frontier_variables, has_event_variables);
    }
    head_variable_index = rule::shared::make_index(head_variables);
    for (auto const &head_atom : head_atoms) {
        std::vector<size_t> var_pos_vector;
        auto const &atom_variables = head_atom->get_variable_names();
        for (auto const &var : atom_variables) {
            auto position = head_variable_index.at(var);
            var_pos_vector.push_back(position);
        }
        head_atoms_var_positions.push_back(var_pos_vector);
    }
}

std::vector<bool> Rule::generate_event_vector() {
    has_event_variables = false;
    size_t bound_variables_count = bound_variables.size();
    std::vector<bool> result;
    for (auto const &var_name : bound_variables) {
        if (event_variables.count(var_name) == 0) {
            result.push_back(false);
        } else {
            result.push_back(true);
            has_event_variables = true;
        }
    }
    return result;
}

void Rule::clear() {
    frontier_variables.clear();
    bound_variables.clear();
    head_atoms.clear();
    head_atoms_var_positions.clear();
    size_t previous_step = 0;
    size_t current_step = 0;
}

void Rule::init_head(
    std::vector<std::unique_ptr<formula::Formula>> head_atoms) {
    this->clear();
    init_chase(head_atoms);
    this->head_atoms = std::move(head_atoms);
    for (auto const &head_atom : this->head_atoms) {
        head_atom->set_head(true);
    }
    trigger_variables = body->get_variable_names();
    for (auto const &math_atom : math_atoms) {
        auto const &math_result_variable = math_atom.get_result_name();
        trigger_variables.push_back(math_result_variable);
    }
}

void Rule::init(std::vector<std::unique_ptr<formula::Formula>> body_atoms,
                std::vector<std::unique_ptr<formula::Formula>> head_atoms) {
    this->body = build_body_formula(0, std::move(body_atoms));
    init_head(std::move(head_atoms));
}

std::unique_ptr<formula::Formula> Rule::build_body_formula(
    size_t index, std::vector<std::unique_ptr<formula::Formula>> list) const {
    if (index == list.size() - 1) {
        auto result = std::move(list[index]);
        return result;
    }
    auto left = std::move(list[index]);
    auto right = build_body_formula(index + 1, std::move(list));
    return std::make_unique<formula::Conjunction>(std::move(left),
                                                  std::move(right));
}

bool Rule::derive_conclusions(util::Timeline const &timeline,
                              util::Database &database) {
    bool result = false;
    std::vector<std::shared_ptr<util::Grounding>> body_groundings =
        body->get_groundings(timeline);
    evaluate_head(timeline, database, body_groundings);
    return true;
}

void Rule::evaluate_head(
    util::Timeline const &timeline, util::Database &database,
    std::vector<std::shared_ptr<util::Grounding>> &body_facts) {
    chase_filter->update(timeline, previous_step, database);
    evaluate_math_atoms(body_facts);
    auto head_facts =
        chase_filter->build_chase_facts(timeline, previous_step, body_facts);
    evaluate_head_atoms(timeline, head_facts);
}

void Rule::evaluate_head_atoms(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> const &body_facts) {
    for (size_t head_atom_index = 0; head_atom_index < head_atoms.size();
         head_atom_index++) {
        auto &head_atom = head_atoms.at(head_atom_index);
        size_t atom_arity = head_atom->get_variable_names().size();
        std::vector<std::shared_ptr<util::Grounding>> head_facts;
        for (auto const &grounding : body_facts) {
            auto head_fact =
                make_atom_fact(grounding, head_atom_index, atom_arity);
            head_facts.push_back(head_fact);
        }
        if (!head_facts.empty()) {
            head_atom->evaluate(timeline, previous_step, head_facts);
        }
    }
}

std::shared_ptr<util::Grounding>
Rule::make_atom_fact(std::shared_ptr<util::Grounding> const &body_fact,
                     size_t head_atom_index, size_t atom_arity) {
    auto const &atom = head_atoms.at(head_atom_index);
    auto const &atom_variables = atom->get_variable_names();
    auto const &predicate = atom->get_predicate_vector().at(0);
    auto const &var_pos_vector = head_atoms_var_positions.at(head_atom_index);
    auto result = body_fact->empty_clone(predicate, atom_arity);
    for (size_t position = 0; position < atom_arity; position++) {
        auto body_position = var_pos_vector.at(position);
        auto const &value = body_fact->get_constant(body_position);
        result->push_constant(value);
    }
    result->set_step(current_step);
    return result;
}

std::vector<std::string> const &Rule::get_frontier_variables() const {
    return frontier_variables;
}

std::vector<std::string> const &Rule::get_bound_variables() const {
    return bound_variables;
}
void Rule::evaluate_math_atoms(
    std::vector<std::shared_ptr<util::Grounding>> &body_facts) {
    for (auto &math_atom : math_atoms) {
        math_atom.evaluate(body, body_facts);
    }
}

} // namespace laser::rule
