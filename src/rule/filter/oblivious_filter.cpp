#include "rule/filter/oblivious_filter.h"

namespace laser::rule {

//ChaseFilter *ObliviousFilter::create() const {
    //auto result = new ObliviousFilter();
    //return result;
//}

//ChaseFilter *ObliviousFilter::clone() const {
    //auto result = new ObliviousFilter();
    //return result;
//}

//ChaseFilter *ObliviousFilter::move() {
    //auto result = new ObliviousFilter();
    //return result;
//}

void ObliviousFilter::init(
    std::vector<std::unique_ptr<formula::Formula>> const &head_atoms,
    std::vector<std::string> const &head_variables,
    std::vector<std::string> const &free_variables,
    std::vector<std::string> const &bound_variables,
    std::vector<bool> const &is_inertia_variable,
    std::vector<std::string> const &frontier_variables,
    bool has_inertia_variables) {
    return;
}

void ObliviousFilter::update(util::Timeline const &timeline,
                             size_t previous_step,
                             util::Database const &database) {
    return;
}

std::vector<std::shared_ptr<util::Grounding>>
ObliviousFilter::build_chase_facts(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &input_facts) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    auto current_time = timeline.get_time();
    for (auto const &input_fact : input_facts) {
        if (input_fact->is_fresh_sne(current_time, previous_step)) {
            result.push_back(input_fact);
        }
    }
    return result;
}

void ObliviousFilter::expire_outdated_groundings(
    util::Timeline const &timeline) {}

} // namespace laser::rule
