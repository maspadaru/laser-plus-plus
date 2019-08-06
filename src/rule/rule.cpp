#include "rule/rule.h"

namespace laser {
namespace rule {

Rule::Rule(formula::Formula *body_formula,
           std::vector<formula::Formula *> head_atoms)
    : body(body_formula->clone()) {
    init(std::move(head_atoms));
}

Rule::~Rule() {
    delete &body;
    for (auto head_atom : head_atoms) {
        delete head_atom;
    }
    head_atoms.clear();
    delete chase_filter;
}

Rule::Rule(Rule const &other) : body(other.body.clone()) {
    head_atoms = other.head_atoms;
    head_atoms_var_positions = other.head_atoms_var_positions;
    chase_filter = other.chase_filter->clone();
    head_variable_index = other.head_variable_index;
    is_existential_m = other.is_existential_m;
    previous_step = other.previous_step;
    current_step = other.current_step;
}

Rule::Rule(Rule &&other) noexcept : body(other.body.move()) {
    head_atoms = std::move(other.head_atoms);
    head_atoms_var_positions = std::move(other.head_atoms_var_positions);
    chase_filter = other.chase_filter->move();
    head_variable_index = std::move(other.head_variable_index);
    is_existential_m = other.is_existential_m;
    previous_step = other.previous_step;
    current_step = other.current_step;
}

Rule &Rule::operator=(Rule const &other) {
    this->body = other.body.clone();
    this->head_atoms = other.head_atoms;
    this->head_atoms_var_positions = other.head_atoms_var_positions;
    this->chase_filter = other.chase_filter->clone();
    this->head_variable_index = other.head_variable_index;
    this->is_existential_m = other.is_existential_m;
    this->previous_step = other.previous_step;
    this->current_step = other.current_step;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->body = other.body.move();
    this->head_atoms = std::move(other.head_atoms);
    this->head_atoms_var_positions = std::move(other.head_atoms_var_positions);
    this->chase_filter = other.chase_filter->move();
    this->head_variable_index = std::move(other.head_variable_index);
    this->is_existential_m = other.is_existential_m;
    this->previous_step = other.previous_step;
    this->current_step = other.current_step;
    return *this;
}

bool Rule::is_existential() const { return is_existential_m; }

void Rule::reset_previous_step() {
    previous_step = 0;
    current_step = 0;
}

void Rule::set_previous_step(size_t step) { previous_step = step; }

void Rule::set_current_step(size_t step) { current_step = step; }

std::vector<std::shared_ptr<util::Grounding>>
Rule::get_conclusions_step(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto head_atom : head_atoms) {
        auto head_groundings = head_atom->get_conclusions_step(timeline);
        result.insert(result.end(), head_groundings.begin(),
                      head_groundings.end());
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Rule::get_conclusions_timepoint(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto head_atom : head_atoms) {
        auto head_groundings = head_atom->get_conclusions_timepoint(timeline);
        result.insert(result.end(), head_groundings.begin(),
                      head_groundings.end());
    }
    return result;
}

void Rule::expire_outdated_groundings(util::Timeline const &timeline) {
    expire_head_groundings(timeline);
    body.expire_outdated_groundings(timeline);
    chase_filter->expire_outdated_groundings(timeline);
}

void Rule::expire_head_groundings(util::Timeline const &timeline) {
    for (auto atom : head_atoms) {
        atom->expire_outdated_groundings(timeline);
    }
}

void Rule::evaluate(util::Timeline const &timeline,
                    util::Database const &database) {
    auto facts = database.get_data_since(previous_step);
    body.evaluate(timeline, previous_step, facts);
}

void Rule::init_chase(std::vector<formula::Formula *> const &head_atoms) {
    std::vector<std::string> free_variables = body.get_variable_names();
    std::set<std::string> free_variable_set(free_variables.begin(),
                                            free_variables.end());
    std::vector<std::string> head_variables;
    head_variables = free_variables;
    std::vector<std::string> bound_variables;
    std::set<std::string> bound_variable_set;
    for (auto atom : head_atoms) {
        auto const &variable_names = atom->get_variable_names();
        for (auto const &var_name : variable_names) {
            if (free_variable_set.count(var_name) == 0) {
                bound_variable_set.insert(var_name);
            }
        }
    }
    if (bound_variable_set.empty()) {
        is_existential_m = false;
        chase_filter = new ObliviousFilter();
    } else {
        is_existential_m = true;
        std::copy(bound_variable_set.begin(), bound_variable_set.end(),
                  std::back_inserter(bound_variables));
        head_variables.insert(head_variables.end(), bound_variables.begin(),
                              bound_variables.end());
        auto chase_algorithm =
            util::Settings::get_instance().get_chase_algorithm();
        switch (chase_algorithm) {
        case util::ChaseAlgorithm::RESTRICTED:
            chase_filter = new RestrictedFilter();
            break;
        case util::ChaseAlgorithm::INDEXED:
            chase_filter = new IndexedFilter();
            break;
        case util::ChaseAlgorithm::SKOLEM:
            chase_filter = new SkolemFilter();
            break;
        default:
            chase_filter = new ObliviousFilter();
            break;
        }
        chase_filter->init(head_atoms, head_variables, free_variables,
                           bound_variables);
    }
    head_variable_index = rule::shared::make_index(head_variables);
    for (auto head_atom : head_atoms) {
        std::vector<size_t> var_pos_vector;
        auto const &atom_variables = head_atom->get_variable_names();
        for (auto const &var : atom_variables) {
            auto position = head_variable_index.at(var);
            var_pos_vector.push_back(position);
        }
        head_atoms_var_positions.push_back(var_pos_vector);
    }
}

void Rule::init(std::vector<formula::Formula *> head_atoms) {
    init_chase(head_atoms);
    this->head_atoms = std::move(head_atoms);
    for (auto *head_atom : this->head_atoms) {
        head_atom->set_head(true);
    }
}

bool Rule::derive_conclusions(util::Timeline const &timeline,
                              util::Database const &database) {
    bool result = false;
    std::vector<std::shared_ptr<util::Grounding>> body_groundings =
        body.get_groundings(timeline);
    evaluate_head(timeline, database, body_groundings);
    return true;
}

void Rule::evaluate_head(
    util::Timeline const &timeline, util::Database const &database,
    std::vector<std::shared_ptr<util::Grounding>> const &body_facts) {
    chase_filter->update(timeline, previous_step, database);
    auto head_facts =
        chase_filter->build_chase_facts(timeline, previous_step, body_facts);
    evaluate_head_atoms(timeline, head_facts);
}

void Rule::evaluate_head_atoms(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> const &body_facts) {
    for (size_t head_atom_index = 0; head_atom_index < head_atoms.size(); head_atom_index++) {
        auto &head_atom = head_atoms.at(head_atom_index);
        size_t atom_arity = head_atom->get_variable_names().size();
        std::vector<std::shared_ptr<util::Grounding>> head_facts;
        for (auto const &grounding : body_facts) {
            auto head_fact = make_atom_fact(grounding, head_atom_index, atom_arity);
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
    for (size_t position = 0; position < atom_arity; position++ ) {
        auto body_position = var_pos_vector.at(position);  
        auto const &value = body_fact->get_constant(body_position);
        result->push_constant(value);
    }
    result->set_step(current_step);
    return result;
}

} // namespace rule
} // namespace laser
