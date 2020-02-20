#include "util/string_pool.h"

namespace laser::util {

StringPool &StringPool::get_instance() {
    static StringPool instance; // Instantiated on first use.
    return instance;
}

size_t StringPool::add_string(std::string const &str) {
    size_t index = 0;
    for (auto const &pool_str : pool) {
        if (pool_str == str) {
            return index;
        }
        index++;
    }
    pool.push_back(str);
    return pool.size() - 1;
}

std::string const &StringPool::get_string(size_t id) const {
    return pool.at(id);
}

} // namespace laser::util
