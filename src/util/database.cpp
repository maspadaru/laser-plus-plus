#include "util/database.h"

namespace laser {
namespace util {

void Database::clear() {
    clear_predicate_map();
    delta.clear();
    current_step = 0;
}

void Database::insert_facts(
    std::vector<std::shared_ptr<util::Grounding>> facts) {
    insert(0, std::move(facts));
}

void Database::insert(
    size_t step, std::vector<std::shared_ptr<util::Grounding>> groundings) {
    while (delta.size() <= step) {
        delta.emplace_back();
    }
    auto &delta_step = delta.at(step);
    delta_step.insert(delta_step.end(),
                      std::make_move_iterator(groundings.begin()),
                      std::make_move_iterator(groundings.end()));
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

std::vector<std::shared_ptr<util::Grounding>> Database::get_data_full() const {
    return get_data_range(0, current_step);
}
void Database::increment_step() { current_step++; }

void Database::update_predicate_map() {
    auto const &new_facts = delta[current_step];
    for (auto const &fact : delta[current_step]) {
        auto const &predicate = fact->get_predicate();
        predicate_map.try_emplace(predicate);
        auto &list = predicate_map.at(predicate);
        list.push_back(fact);
    }
}

void Database::clear_predicate_map() {
    inertia_predicate_map = std::move(predicate_map);
    for (auto &iterator : predicate_map) {
        auto &vector = iterator.second;
        vector.clear();
    }
}

std::vector<std::shared_ptr<util::Grounding>> *
Database::get_predicate_data(std::string const &predicate) {
    predicate_map.try_emplace(predicate);
    auto &result = predicate_map.at(predicate);
    return &result;
}

std::vector<std::shared_ptr<util::Grounding>> *
Database::get_inertia_predicate_data(std::string const &predicate) {
    inertia_predicate_map.try_emplace(predicate);
    auto &result = inertia_predicate_map.at(predicate);
    return &result;
}

size_t Database::get_step() const { return current_step; }

} // namespace util
} // namespace laser
