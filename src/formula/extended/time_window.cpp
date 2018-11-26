#include "formula/extended/time_window.h"

namespace laser {
namespace formula {

TimeWindow::~TimeWindow() { delete child; }

Formula &TimeWindow::create() const {
    auto result = new TimeWindow();
    return *result;
}

Formula &TimeWindow::clone() const {
    auto *result = new TimeWindow(past_size, future_size, step_size,
                                  &this->child->clone());
    result->pivot_time = this->pivot_time;
    return *result;
}

Formula &TimeWindow::move() {
    auto result =
        new TimeWindow(past_size, future_size, step_size, &this->child->move());
    result->pivot_time = this->pivot_time;
    return *result;
}

// getters / setters

void TimeWindow::set_head(bool is_head) {
    child->set_head(is_head);
}

bool TimeWindow::is_head() const {
    return child->is_head();
}

FormulaType TimeWindow::get_type() const { return FormulaType::TIME_WINDOW; }

std::vector<std::string> TimeWindow::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> TimeWindow::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> TimeWindow::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int TimeWindow::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool TimeWindow::is_satisfied() const { return child->is_satisfied(); }

size_t TimeWindow::get_number_of_variables() const {
    return child->get_number_of_variables();
}

util::Timeline TimeWindow::alter_timeline(util::Timeline timeline) const {
    uint64_t window_min_time = timeline.substract(timeline.get_time(), past_size);
    if (timeline.get_min_time() < window_min_time) {
        timeline.set_min_time(window_min_time);
    }
    uint64_t window_max_time = timeline.get_time() + future_size;
    if (timeline.get_max_time() > window_max_time) {
        timeline.set_max_time(window_max_time);
    }
    return timeline;
}

bool TimeWindow::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> facts) {
    auto window_timeline = alter_timeline(timeline);
    return child->evaluate(window_timeline, facts);
}

void TimeWindow::expire_outdated_groundings(util::Timeline timeline) {
    auto window_timeline = alter_timeline(timeline);
    child->expire_outdated_groundings(window_timeline);
}

uint64_t TimeWindow::compute_horizon_time(uint64_t grounding_consideration_time,
                                          uint64_t grounding_horizon_time,
                                          uint64_t current_time) const {
    // TODO: current implementation does not take into account future_size and
    // step

    // The grounding's lifetime should never excede the size of the window
    uint64_t size_limit = grounding_consideration_time + past_size;
    uint64_t result = (size_limit > grounding_horizon_time)
                          ? grounding_horizon_time
                          : size_limit;
    return result;
}

std::vector<Grounding>
TimeWindow::get_groundings(util::Timeline timeline) {
    auto window_timeline = alter_timeline(timeline);
    auto child_conclusions = child->get_groundings(window_timeline);
    for (auto &grounding : child_conclusions) {
        uint64_t window_horizon_time = compute_horizon_time(
            grounding.get_consideration_time(), grounding.get_horizon_time(),
            timeline.get_time());
        grounding.set_horizon_time(window_horizon_time);
    }
    return child_conclusions;
}

std::vector<Grounding>
TimeWindow::get_conclusions_timepoint(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::vector<Grounding>
TimeWindow::get_conclusions_step(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::string TimeWindow::debug_string() const { return child->debug_string(); }

TimeWindow::TimeWindow(uint64_t size, Formula *child) {
    this->past_size = size;
    this->future_size = 0;
    this->step_size = 0;
    this->pivot_time = 0;
    this->child = &child->move();
}

TimeWindow::TimeWindow(uint64_t past_size, uint64_t future_size,
                       uint64_t step_size, Formula *child) {
    this->past_size = past_size;
    this->future_size = future_size;
    this->step_size = step_size;
    this->pivot_time = 0;
    this->child = &child->move();
}

void TimeWindow::add_child(formula::Formula *child) {}

} // namespace formula
} // namespace laser
