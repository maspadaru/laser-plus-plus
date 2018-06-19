#include <iostream>
#include <rule/rule.h>

#include "formula/extended/atom.h"
#include "formula/formula.h"

//// this works because local_atom gets moved when the function returns
//laser::formula::Atom make_atom() {
//    std::string world = "world";
//    laser::formula::Atom local_atom = laser::formula::Atom(world);
//    return local_atom;
//}
//
//int main() {
//    laser::formula::Atom atom = make_atom();
//    laser::formula::Formula& formula = atom;
//    std::cout << "Hello, "
//            << formula.get_predicate() << "!" << std::endl;
//}


/*
 * In the following example if local_atom is not instantiated using new, it's
 * value will be lost at the end of the execution of make_rule().
 * Since *local_atom is allocated on the heap, it will not be lost at the end of
 * make_rule(), and the Rule object returned by the function will not have a
 * null pointer as a head member.
 */
laser::rule::Rule make_rule() {
    std::string world = "world";
    laser::formula::Atom *local_atom = new laser::formula::Atom(world);
    laser::formula::Formula &local_formula = *local_atom;
    laser::rule::Rule local_rule = laser::rule::Rule(&local_formula,
            std::vector<laser::formula::Formula*>());

    laser::formula::Formula &formula = local_rule.get_head();
    std::cout << "FIRST Hello, "
              << formula.get_predicate() << "!" << std::endl;

    return local_rule;
}

int main() {

    laser::rule::Rule rule = make_rule();
    laser::formula::Formula &formula = rule.get_head();

    std::cout << "SECOND Hello, "
              << formula.get_predicate() << "!" << std::endl;
    // delete &formula; -> error: because rule deconstructor tries to free it.
}
