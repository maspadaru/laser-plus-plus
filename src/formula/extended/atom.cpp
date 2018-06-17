//
// Created by mike on 6/15/18.
//

#include <formula/extended/atom.h>

using std::string;

namespace laser {
namespace formula {

void Atom::print_me() {
    std::cout << "Hello, " << this->predicate <<"!" << std::endl;
}

Atom::Atom(string predicate) {
    this->predicate = predicate;
}

} // namespace formula
} // namespace laser

