#include "formula/extended/tuple_window.h"

namespace laser::formula {

TupleWindow::~TupleWindow() { delete child; }

Formula &TupleWindow::create() const {
    auto result = new TupleWindow();
    return *result;
}

Formula &TupleWindow::clone() const {
    auto *result =
        new TupleWindow(past_size, future_size, &this->child->clone());
    return *result;
}

Formula &TupleWindow::move() {
    auto result = new TupleWindow(past_size, future_size, &this->child->move());
    return *result;
}

void TupleWindow::set_head(bool is_head) { child->set_head(is_head); }

void TupleWindow::set_extensional_predicates(
    std::set<std::string> const &extensional_predicates) {
    child->set_extensional_predicates(extensional_predicates);
}

bool TupleWindow::is_head() const { return child->is_head(); }

FormulaType TupleWindow::get_type() const { return FormulaType::TUPLE_WINDOW; }

std::vector<std::string> const &TupleWindow::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::map<std::string, size_t> const &TupleWindow::get_arity_map() const {
    return child->get_arity_map();
}

std::vector<std::string> const &TupleWindow::get_variable_names() const {
    return child->get_variable_names();
}

int TupleWindow::get_variable_index(std::string const &variable_name) const {
    return child->get_variable_index(variable_name);
}

size_t TupleWindow::get_number_of_variables() const {
    return child->get_number_of_variables();
}

std::vector<std::shared_ptr<util::Grounding>> TupleWindow::adjust_annotations(
    std::vector<std::shared_ptr<util::Grounding>> const &facts) const {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &fact : facts) {
        // TODO check if fact->get_predicate in child->get_predicate_vector();
        auto horizon_count = compute_horizon_count(
            fact->get_consideration_count(), fact->get_horizon_count());
        auto grounding = fact->clone();
        grounding->set_horizon_count(horizon_count);
        result.push_back(std::move(grounding));
    }
    return result;
}

util::Timeline TupleWindow::alter_timeline(util::Timeline timeline) const {
    auto time = timeline.get_time();
    auto current_tuple_count = timeline.get_tuple_count_at(time);
    if (current_tuple_count < past_size) {
        // there are not enought facts in the timeline, so there is no need
        // for croping
        return timeline;
    }

    auto low_tuple_count = timeline.substract(current_tuple_count, past_size);
    auto current_time = timeline.get_time();
    auto test_time = current_time;
    bool low_time_found = false;
    while (!low_time_found) {
        auto test_tuple_count = timeline.get_tuple_count_at(test_time);
        if (test_tuple_count <= low_tuple_count) {
            low_time_found = true;
            test_time++;
        } else {
            test_time--;
        }
    }
    auto low_time = timeline.max(timeline.get_min_time(), test_time);
    timeline.set_min_time(low_time);
    return timeline;
}

bool TupleWindow::evaluate(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    auto child_facts = adjust_annotations(facts);
    auto window_timeline = alter_timeline(timeline);
    return child->evaluate(window_timeline, previous_step, child_facts);
}

void TupleWindow::expire_outdated_groundings(util::Timeline const &timeline) {
    auto window_timeline = alter_timeline(timeline);
    child->expire_outdated_groundings(window_timeline);
}

uint64_t
TupleWindow::compute_horizon_count(uint64_t grounding_consideration_count,
                                   uint64_t grounding_horizon_count) const {
    // TODO: current implementation does not take into account future_size
    uint64_t new_hc = grounding_consideration_count + past_size;
    uint64_t result =
        (new_hc < grounding_horizon_count) ? new_hc : grounding_horizon_count;
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
TupleWindow::get_groundings(util::Timeline const &timeline) {
    return child->get_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
TupleWindow::get_conclusions_timepoint(util::Timeline const &timeline) {
    return get_groundings(timeline);
}

std::vector<std::shared_ptr<util::Grounding>>
TupleWindow::get_conclusions_step(util::Timeline const &timeline) {
    return get_groundings(timeline);
}

TupleWindow::TupleWindow(uint64_t size, Formula *child) {
    this->past_size = size;
    this->future_size = 0;
    this->child = &child->move();
}

TupleWindow::TupleWindow(uint64_t past_size, uint64_t future_size,
                         Formula *child) {
    this->past_size = past_size;
    this->future_size = future_size;
    this->child = &child->move();
}

void TupleWindow::add_child(formula::Formula *child) {}

std::vector<formula::Formula *> TupleWindow::get_children() const {
    std::vector<formula::Formula *> result;
    result.push_back(child);
    return result;
} 

uint64_t TupleWindow::get_window_size() const {
    return past_size;
} 

} // namespace laser::formula
