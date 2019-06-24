#ifndef LASER_UTIL_SETTINGS_H
#define LASER_UTIL_SETTINGS_H

#include "util/chase_algorithm.h"

namespace laser {
namespace util {

class Settings {
  private:
    ChaseAlgorithm chase_algorithm = ChaseAlgorithm::OBLIVIOUS;  

    Settings() {} 

  public:
    Settings(Settings const&) = delete;

    void operator=(Settings const&) = delete;

    static Settings &get_instance();

    ChaseAlgorithm get_chase_algorithm() const;

    void set_chase_algorithm(ChaseAlgorithm chase_algorithm);
};

} // namespace util
} // namespace laser

#endif // LASER_UTIL_SETTINGS_H
