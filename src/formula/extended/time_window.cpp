#include "formula/extended/time_window.h"

namespace laser {
namespace formula {


TimeWindow::~TimeWindow() {

}

Formula &TimeWindow::create() const {

}

Formula &TimeWindow::clone() const {

}

Formula &TimeWindow::move() {

}

// getters / setters

FormulaType TimeWindow::get_type() const {

}

std::vector<std::string> TimeWindow::get_predicate_vector() const {

}

// methods

std::vector<std::string> TimeWindow::get_variable_names() const {

}

std::vector<std::string> TimeWindow::get_full_variable_names() const {

}

int TimeWindow::get_variable_index(std::string variable_name) const {

}

bool TimeWindow::is_satisfied() const {

}

bool TimeWindow::evaluate(
    uint64_t current_time, uint64_t current_tuple_counter,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {

}

size_t TimeWindow::get_number_of_variables() const {

}

void TimeWindow::expire_outdated_groundings(uint64_t current_time,
                                        uint64_t current_tuple_counter) {

}

std::vector<Grounding> TimeWindow::get_groundings() const {

}

std::string TimeWindow::debug_string() const {

}


} // namespace formula
} // namespace laser
