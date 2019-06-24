#ifndef LASER_RULE_SHARED_H
#define LASER_RULE_SHARED_H

#include <cstdint>
#include <memory>

#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser {
namespace rule {
namespace shared {

std::unordered_map<std::string, int>
make_index(std::vector<std::string> const &vector);

} // namespace shared
} // namespace rule
} // namespace laser

#endif // LASER_RULE_SHARED_H
