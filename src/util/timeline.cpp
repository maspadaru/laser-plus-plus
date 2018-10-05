#include "util/timeline.h"

namespace laser {
namespace util {

const uint64_t Timeline::INFINITE_TIME = ULLONG_MAX;   

uint64_t Timeline::get_time() const {
    return time;
}

void Timeline::set_time(uint64_t time) {
    this->time = time;
}

void Timeline::increment_time() {
    time++;
}

uint64_t Timeline::get_tuple_count() const {
    return tuple_count;
}


void Timeline::set_tuple_count(uint64_t tuple_count) {
    this->tuple_count = tuple_count;
}
void Timeline::increment_tuple_count() {
    tuple_count++;
}

uint64_t Timeline::get_min_time() const {
    return min_time;
}


void Timeline::set_min_time(uint64_t min_time) {
    this->min_time = min_time;
}

uint64_t Timeline::get_max_time() const {
    return max_time;
}
void Timeline::set_max_time(uint64_t max_time) {
    this->max_time = max_time;
}

uint64_t Timeline::get_min_tuple_count() const {
    return min_tuple_count;
}


void Timeline::set_min_tuple_count(uint64_t min_tuple_count) {
    this->min_tuple_count = min_tuple_count;
}

uint64_t Timeline::get_max_tuple_count() const {
    return max_tuple_count;
}


void Timeline::set_max_tuple_count(uint64_t max_tuple_count) {
    this->max_tuple_count = max_tuple_count;
}


bool Timeline::is_past_max_time() const {
    return time >= max_time;
}

bool Timeline::is_past_max_tuple_count() const {
    return tuple_count >= max_tuple_count;
}

} // namespace util
} // namespace laser
