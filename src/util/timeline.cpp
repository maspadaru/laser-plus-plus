#include "util/timeline.h"

namespace laser::util {

Timeline::Timeline()
    : history_mutex(std::make_shared<std::mutex>()), tuple_count_history(std::make_shared<std::unordered_map<uint64_t, uint64_t>>()) {}

const uint64_t Timeline::INFINITE_TIME = ULLONG_MAX;

void Timeline::set_start_time(uint64_t initial_time) {
    // make sure this is executed only when initializing the Timeline
    if (time == 0) {
        time = initial_time;
    }
}

uint64_t Timeline::get_time() const { return time; }

void Timeline::set_time(uint64_t time) { this->time = time; }

void Timeline::increment_time() { time++; }
void Timeline::decrement_time() { time--; }

void Timeline::set_tuple_count(uint64_t tuple_count) {
    std::lock_guard<std::mutex> guard(*history_mutex);
    this->tuple_count = tuple_count;
    tuple_count_history->try_emplace(time, tuple_count);
}

uint64_t Timeline::get_min_time() const { return min_time; }

void Timeline::set_min_time(uint64_t min_time) { this->min_time = min_time; }

uint64_t Timeline::get_max_time() const { return max_time; }
void Timeline::set_max_time(uint64_t max_time) { this->max_time = max_time; }

uint64_t Timeline::get_tuple_count_at(uint64_t timepoint) const {
    std::lock_guard<std::mutex> guard(*history_mutex);
    return tuple_count_history->at(timepoint);
}


bool Timeline::is_past_max_time() const { return time > max_time; }

uint64_t Timeline::substract(uint64_t initial_value,
                             uint64_t substracted_val) const {
    if (substracted_val > initial_value) {
        return 0;
    }
    return initial_value - substracted_val;
}
uint64_t Timeline::min(uint64_t first, uint64_t second) const {
    int result = (first < second) ? first : second;
    return result;
}

uint64_t Timeline::max(uint64_t first, uint64_t second) const {
    int result = (first > second) ? first : second;
    return result;
}

} // namespace util::laser
