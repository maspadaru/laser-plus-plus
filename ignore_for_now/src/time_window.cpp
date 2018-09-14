#include "formula/extended/time_window.h"

namespace laser {
namespace formula {


TimeWindow::~TimeWindow() {
    delete child;
}

Formula &TimeWindow::create() const {
    auto result = new TimeWindow();
    return *result;
}

Formula &TimeWindow::clone() const {
    auto result = new TimeWindow(*this);
    return *result;
}

Formula &TimeWindow::move() {
   auto result = new TimeWindow(std::move(*this));
   return *result;
}

// getters / setters

FormulaType TimeWindow::get_type() const {
    return FormulaType::TIME_WINDOW; 
}

std::vector<std::string> TimeWindow::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> TimeWindow::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> TimeWindow::get_full_variable_names() const {
    return child ->get_full_variable_names();
}

int TimeWindow::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool TimeWindow::is_satisfied() const {
    return child->is_satisfied();
}

bool TimeWindow::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    return child->evaluate(current_time, current_tuple_counter, facts);
}

size_t TimeWindow::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void TimeWindow::expire_outdated_groundings(uint64_t current_time,
                                        uint64_t current_tuple_counter) {
    child->expire_outdated_groundings(current_time, current_tuple_counter);
}

std::vector<Grounding> TimeWindow::get_groundings() const {
    return child->get_groundings();
}

std::string TimeWindow::debug_string() const {
    return child->debug_string();
}


} // namespace formula
} // namespace laser
