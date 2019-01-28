#include "formula/extended/tuple_window.h"

namespace laser {
namespace formula {

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

// getters / setters

void TupleWindow::set_head(bool is_head) { child->set_head(is_head); }

bool TupleWindow::is_head() const { return child->is_head(); }

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

size_t TupleWindow::get_number_of_variables() const {
    return child->get_number_of_variables();
}

util::Timeline TupleWindow::alter_timeline(util::Timeline timeline) const {
    timeline.set_min_tuple_count(timeline.get_tuple_count());
    return timeline;
}

std::unordered_map<std::string, std::vector<std::shared_ptr<Grounding>>>
TupleWindow::adjust_annotations(
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts)
    const {
    std::unordered_map<std::string, std::vector<std::shared_ptr<Grounding>>>
        result;
    auto predicates = child->get_predicate_vector();
    for (auto const &predicate : predicates) {
        auto fact_vector = facts.at(predicate);
        result.try_emplace(predicate);
        auto result_vector = result.at(predicate);
        for (auto const &fact : fact_vector) {
            auto horizon_count = compute_horizon_count(
                fact->get_consideration_count(), fact->get_horizon_count());
            auto grounding = fact->new_horizon_count(horizon_count);
            result_vector.push_back(grounding);
        }
    }
    return result;
}

bool TupleWindow::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    auto window_timeline = alter_timeline(timeline);
    auto child_facts = adjust_annotations(facts);
    return child->evaluate(window_timeline, child_facts);
}

void TupleWindow::expire_outdated_groundings(util::Timeline timeline) {
    auto window_timeline = alter_timeline(timeline);
    child->expire_outdated_groundings(window_timeline);
}

uint64_t
TupleWindow::compute_horizon_count(uint64_t grounding_consideration_count,
                                   uint64_t grounding_horizon_count) const {
    // TODO: current implementation does not take into account future_size 

    // The grounding's lifetime should never excede the size of the window
    uint64_t size_limit = grounding_consideration_count + past_size;
    uint64_t result =
        (size_limit > grounding_horizon_count && grounding_horizon_count > 0)
            ? grounding_horizon_count
            : size_limit;
    return result;
}

std::vector<std::shared_ptr<Grounding>>
TupleWindow::get_groundings(util::Timeline timeline) {
    return child->get_groundings(timeline);
}

std::vector<std::shared_ptr<Grounding>>
TupleWindow::get_conclusions_timepoint(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::vector<std::shared_ptr<Grounding>>
TupleWindow::get_conclusions_step(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::string TupleWindow::debug_string() const { return child->debug_string(); }

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

} // namespace formula
} // namespace laser
