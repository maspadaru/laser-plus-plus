#ifndef LASER_RULE_SHARE_H
#define LASER_RULE_SHARE_H

#include <memory>
#include <cstdint>

#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser {
namespace rule {
namespace share {
    
    bool is_valid_sne(uint64_t now,
                      std::shared_ptr<util::Grounding> const &grounding);

} // namespace share 
} // namespace rule
} // namespace laser

#endif // LASER_RULE_SHARE_H
