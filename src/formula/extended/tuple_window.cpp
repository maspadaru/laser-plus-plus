#include "formula/extended/tuple_window.h"

namespace laser {
namespace formula {

TupleWindow::~TupleWindow() { delete child; }

Formula &TupleWindow::create() const {
    auto result = new TupleWindow();
    return *result;
}

Formula &TupleWindow::clone() const {
    auto result = new TupleWindow(*this);
    return *result;
}

Formula &TupleWindow::move() {
    auto result = new TupleWindow(std::move(*this));
    return *result;
}

// getters / setters

FormulaType TupleWindow::get_type() const { return FormulaType::TUPLE_WINDOW; }

std::vector<std::string> TupleWindow::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> TupleWindow::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> TupleWindow::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int TupleWindow::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool TupleWindow::is_satisfied() const { return child->is_satisfied(); }

bool TupleWindow::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t TupleWindow::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void TupleWindow::expire_outdated_groundings(uint64_t current_time,
                                             uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> TupleWindow::get_groundings() const {
    return child->get_groundings();
}

std::string TupleWindow::debug_string() const { return child->debug_string(); }

} // namespace formula
} // namespace laser
