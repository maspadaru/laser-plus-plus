#include "rule/share.h"

namespace laser {
namespace rule {
namespace share {
    
bool is_valid_sne(
    uint64_t now,
    std::shared_ptr<util::Grounding> const &grounding) {
    // SNE: we only evaluate groundings derived at this current timepoint
    // checking ct >= because facts can be derived at future timepoints from
    // TimeReference formulas in head of rules
    // TODO I sould only get the ones derived at the cureent STEP
    return grounding->get_consideration_time() >= now;
}

} // namespace share 
} // namespace rule
} // namespace laser

