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

bool Settings::has_global_null_values() const {
    return has_global_null_values_m;
}

void Settings::set_global_null_values(bool has_global_nulls) {
    has_global_null_values_m = has_global_nulls;
}

} // namespace util
} // namespace laser
