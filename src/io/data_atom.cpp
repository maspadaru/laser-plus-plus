//
// Created by mike on 7/17/18.
//

#include "io/data_atom.h"

namespace laser {
namespace io {

DataAtom::DataAtom(std::string predicate, std::vector<std::string> arguments)
    : predicate(std::move(predicate)), argument_vector(std::move(arguments)) {}

std::string DataAtom::get_predicate() const { return predicate; }

std::vector<std::string> DataAtom::get_arguments() const {
    return argument_vector;
}

} // namespace io
} // namespace laser
