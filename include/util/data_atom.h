//
// Created by mike on 7/17/18.
//

#ifndef LASER_UTIL_DATA_ATOM_H
#define LASER_UTIL_DATA_ATOM_H

#include <string>
#include <utility>
#include <vector>
#include <memory>

namespace laser {
namespace util {

class DataAtom {
  private:
    std::shared_ptr<std::string const> predicate;
    std::vector<std::shared_ptr<std::string const>> argument_vector;

  public:
    // constructors & destructors
    DataAtom(std::shared_ptr<std::string const> predicate, std::vector<std::shared_ptr<std::string const>> arguments);
    ~DataAtom() = default;

    // getters & setters
    std::shared_ptr<std::string const> get_predicate() const;

    std::vector<std::shared_ptr<std::string const>> get_arguments() const;

    // methods
};

} // namespace util
} // namespace laser

#endif // LASER_UTIL_DATA_ATOM_H
