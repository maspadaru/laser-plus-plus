#ifndef LASER_UTIL_GLOBAL_H
#define LASER_UTIL_GLOBAL_H

#include <cstdint>

namespace laser::util {

class Global {
  private:
    uint64_t null_value_count = 0;

    Global() {} 

  public:
    Global(Global const&) = delete;

    void operator=(Global const&) = delete;

    static Global &get_instance();

    uint64_t get_null_value_count();

};

} // namespace laser::util

#endif // LASER_UTIL_GLOBAL_H
