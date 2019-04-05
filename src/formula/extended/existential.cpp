#include "formula/extended/existential.h"

namespace laser {
namespace formula {

Existential::~Existential() { delete child; }

Existential::Existential(std::vector<std::string> argument_vector, Formula *child) {
    this->quantified_variables = std::move(argument_vector);
    this->child = &child->move();
}

Formula &Existential::create() const {
    auto result = new Existential();
    return *result;
}

Formula &Existential::clone() const {
    auto *result = new Existential(this->quantified_variables, &this->child->clone());
    return *result;
}

Formula &Existential::move() {
    auto result =
        new Existential(std::move(this->quantified_variables), &this->child->move());
    return *result;
}

void Existential::set_head(bool is_head) { child->set_head(is_head); }

bool Existential::is_head() const { return child->is_head(); }

FormulaType Existential::get_type() const { return FormulaType::EXISTENTIAL; }

std::vector<std::string> Existential::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::vector<std::string> const &Existential::get_variable_names() const {
    return child->get_variable_names();
}

int Existential::get_variable_index(std::string const &variable_name) const {
    return child->get_variable_index(variable_name);
}

size_t Existential::get_number_of_variables() const {
    return child->get_number_of_variables();
}

bool Existential::evaluate(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    return child->evaluate(timeline, facts);
}

void Existential::expire_outdated_groundings(util::Timeline const &timeline) {
    child->expire_outdated_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Existential::get_groundings(util::Timeline const &timeline) {
    auto result = child->get_groundings(timeline);
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Existential::get_conclusions_timepoint(util::Timeline const &timeline) {
    return child->get_conclusions_timepoint(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
Existential::get_conclusions_step(util::Timeline const &timeline) {
    return child->get_conclusions_step(timeline);
}

void Existential::add_child(formula::Formula *child) {}

} // namespace formula
} // namespace laser
