#include <iostream>

#include "formula/extended/atom.h"
#include "formula/formula.h"

int main() {

    laser::formula::Atom atom = laser::formula::Atom("world");
    laser::formula::Formula& formula = (laser::formula::Formula&) atom;

    std::cout << "Hello, "
            << formula.get_predicate() << "!" << std::endl;

    return 0;
}