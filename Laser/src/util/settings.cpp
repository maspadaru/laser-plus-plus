#include "util/settings.h"

namespace laser {
namespace util {

Settings &Settings::get_instance() {
    static Settings instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
    return instance;
}

ChaseAlgorithm Settings::get_chase_algorithm() const { return chase_algorithm; }

void Settings::set_chase_algorithm(ChaseAlgorithm chase_algorithm) {
    this->chase_algorithm = chase_algorithm;
}

} // namespace util
} // namespace laser
