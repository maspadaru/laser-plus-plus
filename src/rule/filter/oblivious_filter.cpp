#include "rule/filter/oblivious_filter.h"

namespace laser {
namespace rule {

ChaseFilter *ObliviousFilter::create() const {
    auto result = new ObliviousFilter();
    return result;
}

ChaseFilter *ObliviousFilter::clone() const {
    auto result = new ObliviousFilter();
    return result;
}

ChaseFilter *ObliviousFilter::move() {
    auto result = new ObliviousFilter();
    return result;
}

void ObliviousFilter::init(std::vector<formula::Formula *> const &head_atoms) {
    return;
}

void ObliviousFilter::update(util::Timeline const &timeline,
                             util::Database const &database) {
    return;
}

std::vector<std::shared_ptr<util::Grounding>>
ObliviousFilter::build_chase_facts(
    util::Timeline const &timeline,
    std::vector<std::shared_ptr<util::Grounding>> const &input_facts) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    auto current_time = timeline.get_time();
    for (auto const &input_fact : input_facts) {
        if (rule::share::is_valid_sne(current_time, input_fact)) {
            result.push_back(input_fact);
        }
    }
    return result;
}

} // namespace rule
} // namespace laser
