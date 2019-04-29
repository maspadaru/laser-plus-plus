#include "rule/shared.h"

namespace laser {
namespace rule {
namespace shared {

bool is_valid_sne(uint64_t now,
                  std::shared_ptr<util::Grounding> const &grounding) {
    // SNE: we only evaluate groundings derived at this current timepoint
    // checking ct >= because facts can be derived at future timepoints from
    // TimeReference formulas in head of rules
    // TODO I sould only get the ones derived at the cureent STEP
    return grounding->get_consideration_time() >= now;
}

std::unordered_map<std::string, int>
make_index(std::vector<std::string> const &vector) {
    std::unordered_map<std::string, int> result;
    size_t index = 0;
    for (auto const &var_name : vector) {
        result.try_emplace(var_name, index);
        index++;
    }
    return result;
}

} // namespace share
} // namespace rule
} // namespace laser
