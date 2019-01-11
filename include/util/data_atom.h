//
// Created by mike on 7/17/18.
//

#ifndef LASER_UTIL_DATA_ATOM_H
#define LASER_UTIL_DATA_ATOM_H

#include <string>
#include <utility>
#include <vector>

namespace laser {
namespace util {

class DataAtom {
  private:
    std::string predicate;
    std::vector<std::string> argument_vector;

  public:
    // constructors & destructors
    DataAtom(std::string predicate, std::vector<std::string> arguments);
    ~DataAtom() = default;

    // getters & setters
    std::string get_predicate() const;

    std::vector<std::string> get_arguments() const;

    // methods
};

} // namespace util
} // namespace laser

#endif // LASER_UTIL_DATA_ATOM_H
