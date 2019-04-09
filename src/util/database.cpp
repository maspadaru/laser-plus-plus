#include "util/database.h"

namespace laser {
namespace util {

void Database::clear() { 
    delta.clear();
    current_step = 0;
}

void Database::insert_facts(
    std::vector<std::shared_ptr<util::Grounding>> facts) {
    insert(0, std::move(facts));
}

void Database::insert(
    size_t step, std::vector<std::shared_ptr<util::Grounding>> groundings) {
    delta.insert(delta.begin() + step, std::move(groundings));
}

std::vector<std::shared_ptr<util::Grounding>>
Database::get_data_range(size_t start, size_t end) const {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto i = start; i < end + 1; i++) {
        result.insert(result.end(), delta[i].begin(), delta[i].end());
    }
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Database::get_data_step(size_t step) const {
    return get_data_range(step, step);
}

std::vector<std::shared_ptr<util::Grounding>>
Database::get_data_range(size_t end) const {
    return get_data_range(0, end);
}

std::vector<std::shared_ptr<util::Grounding>>
Database::get_data_since(size_t start) const {
    return get_data_range(start, current_step);
}

std::vector<std::shared_ptr<util::Grounding>>
Database::get_data_full() const {
    return get_data_range(1, current_step);
}
void Database::increment_step() { current_step++; }

size_t Database::get_step() const { return current_step; }

} // namespace util
} // namespace laser
