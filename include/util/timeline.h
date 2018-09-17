//
// Created by mike on 9/14/18.
//

#ifndef LASER_UTIL_TIMELINE_H
#define LASER_UTIL_TIMELINE_H

#include <climits>
#include <cstdint>

namespace laser {
namespace util {

class Timeline {
  private:
    uint64_t time = 0;
    uint64_t tuple_count = 0;
    uint64_t min_time = 0;
    uint64_t max_time = ULLONG_MAX;
    uint64_t min_tuple_count = 0;
    uint64_t max_tuple_count = ULLONG_MAX;

  public:

    Timeline() = default;

    uint64_t get_time() const;
    void set_time(uint64_t time);
    void increment_time();

    uint64_t get_tuple_count() const;
    void set_tuple_count(uint64_t tuple_count);
    void increment_tuple_count();

    uint64_t get_min_time() const;
    void set_min_time(uint64_t min_time);

    uint64_t get_max_time() const;
    void set_max_time(uint64_t max_time);

    uint64_t get_min_tuple_count() const;
    void set_min_tuple_count(uint64_t min_tuple_count);

    uint64_t get_max_tuple_count() const;
    void set_max_tuple_count(uint64_t max_tuple_count);

    bool is_past_max_time() const;

    bool is_past_max_tuple_count() const;


};

// TODO: when some time == ULLONG_MAX it should flip to zero. We should check
// for this when we increment/decrement/set
// TODO: Then we should implement a `compare` function that will be able to detect
// that 1 > ULLONG max, since in this case we can be sure there was a flip
// TODO: Limits don't have to be set. They can be infinite. In this case, we
// should have functions: set_infinite_time(), set_inifinite_tuples(), that set
// some private bool values. When this vals are set, get_end_time() should 
// always return ULLONG_MAX, or something.  

} // namespace util
} // namespace laser

#endif // LASER_UTIL_TIMELINE_H
