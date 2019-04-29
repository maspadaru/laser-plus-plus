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
}

Rule::Rule(Rule const &other)
    : body(other.body.clone()) {
    chase_filter = other.chase_filter;
    head_atoms = other.head_atoms;
}

Rule::Rule(Rule &&other) noexcept
    : body(other.body.move()) {
    chase_filter = std::move(other.chase_filter);
    head_atoms = std::move(other.head_atoms);
}

Rule &Rule::operator=(Rule const &other) {
    this->body = other.body.clone();
    this->chase_filter = other.chase_filter;
    this->head_atoms = other.head_atoms;
    return *this;
}

Rule &Rule::operator=(Rule &&other) noexcept {
    this->body = other.body.move();
    this->chase_filter = std::move(other.chase_filter);
    this->head_atoms = std::move(other.head_atoms);
    return *this;
}

bool Rule::is_existential() const {
    return is_existential_m;
}

void Rule::reset_previous_step() { previous_step = 0; }

void Rule::set_previous_step(size_t step) { previous_step = step; }

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
}

void Rule::expire_head_groundings(util::Timeline const &timeline) {
    for (auto atom : head_atoms) {
        atom->expire_outdated_groundings(timeline);
    }
}

void Rule::evaluate(util::Timeline const &timeline,
                    util::Database const &database) {
    auto facts = database.get_data_since(previous_step);
    body.evaluate(timeline, database, facts);
}

void Rule::init_chase(std::vector<formula::Formula *> const &head_atoms) {
    // TODO check if all variables from head are found in body and generate maps
    // and set is_existential_m
    //
    chase_filter = new ObliviousFilter();
    chase_filter->init(head_atoms);
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
    chase_filter->update(timeline, database);
    auto head_facts = chase_filter->build_chase_facts(timeline, body_facts); 
    evaluate_head_atoms(timeline, database, head_facts);
}

void Rule::evaluate_head_atoms(
    util::Timeline const &timeline, util::Database const &database,
    std::vector<std::shared_ptr<util::Grounding>> const &body_facts) {
    for (auto head_atom : head_atoms) {
        std::vector<std::shared_ptr<util::Grounding>> head_facts;
        for (auto const &grounding : body_facts) {
            auto head_fact = make_atom_fact(grounding, head_atom);
            head_facts.push_back(head_fact);
        }
        if (!head_facts.empty()) {
            head_atom->evaluate(timeline, database, head_facts);
        }
    }
}

std::shared_ptr<util::Grounding>
Rule::make_atom_fact(std::shared_ptr<util::Grounding> const &body_fact,
                     formula::Formula *atom) const {
    auto atom_variables = atom->get_variable_names();
    auto predicate = atom->get_predicate_vector().at(0);
    std::vector<std::string> atom_values;
    for (auto const &var : atom_variables) {
        auto position = body.get_variable_index(var);
        auto const &value = body_fact->get_constant(position);
        atom_values.push_back(value);
    }
    return body_fact->new_pred_constvec(predicate, atom_values);
}

} // namespace rule
} // namespace laser
