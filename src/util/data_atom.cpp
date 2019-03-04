//
// Created by mike on 7/17/18.
//

#include "util/data_atom.h"

namespace laser {
namespace util {

DataAtom::DataAtom(std::string predicate, std::vector<std::string> arguments)
    : predicate(std::move(predicate)), argument_vector(std::move(arguments)) {}

std::string const &DataAtom::get_predicate() const { return predicate; }

std::vector<std::string> const &DataAtom::get_arguments() const {
    return argument_vector;
}

} // namespace util
} // namespace laser
