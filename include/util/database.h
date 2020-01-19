#ifndef LASER_UTIL_DATABASE_H
#define LASER_UTIL_DATABASE_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "util/grounding.h"

namespace laser {
namespace util {

class Database {
  private:
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<util::Grounding>>>
        predicate_map;


    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<util::Grounding>>>
        inertia_predicate_map;

    std::vector<std::vector<std::shared_ptr<util::Grounding>>> delta;
    size_t current_step = 0;

    void clear_predicate_map(); 

  public:
    void increment_step();
    void clear();
    void insert_facts(std::vector<std::shared_ptr<util::Grounding>> facts);
    void insert(size_t step,
                std::vector<std::shared_ptr<util::Grounding>> groundings);
    std::vector<std::shared_ptr<util::Grounding>>
    get_data_step(size_t step) const;
    std::vector<std::shared_ptr<util::Grounding>>
    get_data_range(size_t start, size_t end) const;
    std::vector<std::shared_ptr<util::Grounding>>
    get_data_range(size_t end) const;
    std::vector<std::shared_ptr<util::Grounding>>
    get_data_since(size_t start) const;
    std::vector<std::shared_ptr<util::Grounding>> get_data_full() const;
    size_t get_step() const;

    void update_predicate_map();

    std::vector<std::shared_ptr<util::Grounding>>  
    *get_predicate_data(std::string const &predicate);

    std::vector<std::shared_ptr<util::Grounding>>  
    *get_inertia_predicate_data(std::string const &predicate);
};

} // namespace util
} // namespace laser

#endif // LASER_UTIL_DATABASE_H
