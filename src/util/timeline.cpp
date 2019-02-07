#include "util/timeline.h"

namespace laser {
namespace util {

const uint64_t Timeline::INFINITE_TIME = ULLONG_MAX;   

void Timeline::set_start_time(uint64_t initial_time) {
    // make sure this is executed only when initializing the Timeline
    if (time == 0) {
        // TODO 999999999 seems to be the maximum number I can reserver
        // for tuple_count_history. Meaning that I can not track more
        // timepoints. I have to implement timeline in such a way that 
        // after this number, the counter resets to zero and uses modulo
        // operations to figure out which number is larger. 
        tuple_count_history.reserve(999999999);
        // make sure there is no gap in the beggining of the time vector
        if (initial_time > 0) {
            for (uint64_t i = 0; i < initial_time; i++) {
                tuple_count_history.push_back(0);
            }
        }
        time = initial_time;    
    }
}

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
    tuple_count_history.push_back(tuple_count);
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

uint64_t Timeline::get_tuple_count_at(uint64_t timepoint) const {
    return tuple_count_history[timepoint];
}

//uint64_t Timeline::get_min_tuple_count() const {
    //return min_tuple_count;
//}


//void Timeline::set_min_tuple_count(uint64_t min_tuple_count) {
    //this->min_tuple_count = min_tuple_count;
//}

//uint64_t Timeline::get_max_tuple_count() const {
    //return max_tuple_count;
//}


//void Timeline::set_max_tuple_count(uint64_t max_tuple_count) {
    //this->max_tuple_count = max_tuple_count;
//}


bool Timeline::is_past_max_time() const {
    return time > max_time;
}

//bool Timeline::is_past_max_tuple_count() const {
    //return tuple_count >= max_tuple_count;
//}

uint64_t Timeline::substract(uint64_t initial_value, uint64_t substracted_val) const {
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

} // namespace util
} // namespace laser
