#include <iostream>
#include <rule/rule.h>

#include "formula/extended/atom.h"
#include "formula/formula.h"

//==============================================================================

// this works because local_atom gets moved when the function returns
laser::formula::Atom make_atom() {
    std::string world = "world";
    laser::formula::Atom local_atom = laser::formula::Atom(world);
    return local_atom;
}

int main() {
    laser::formula::Atom atom = make_atom();
    laser::formula::Formula& formula = atom;
    std::cout << "Hello, "
            << formula.get_predicate() << "!" << std::endl;
}

//==============================================================================

/*
 * In the following example if local_atom is not instantiated using new, it's
 * value will be lost at the end of the execution of make_rule().
 * Since *local_atom is allocated on the heap, it will not be lost at the end of
 * make_rule(), and the Rule object returned by the function will not have a
 * null pointer as a head member.
 *
 * But still this can go wrong, as make_rule() will return a shallow copy of
 * alocal_rule nd then call delete local_rule, which deletes local_atom.
 *
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


//==============================================================================

/*
 * This works if the Rule and Atom objects are instantiated on the heap using
 * "new". Otherwise I get a segmentation fault.
 */
laser::rule::Rule* make_rule() {
    std::string world = "world";
    laser::formula::Atom *local_atom = new laser::formula::Atom(world);
    laser::formula::Formula *local_formula = local_atom;
    laser::rule::Rule *local_rule = new laser::rule::Rule(local_formula,
            std::vector<laser::formula::Formula*>());

    laser::formula::Formula *formula = local_rule->get_head();
    std::cout << "FIRST Hello, "
              << formula->get_predicate() << "!" << std::endl;

    return local_rule;
}

std::vector<laser::rule::Rule*> make_rule_vector(int n) {
    std::vector<laser::rule::Rule*> vector;
    for (int i = 0; i < n; i++){
        auto rule = make_rule();
        laser::formula::Formula *formula = rule->get_head();
        std::cout << "SECOND Hello, "
                  << formula->get_predicate() << "!" << std::endl;
        vector.push_back(rule);
    }
    return vector;
}

int main() {

    int n = 5;
    auto rule_vector = make_rule_vector(n);
    for (int i = 0; i < n; i++) {
        laser::rule::Rule *rule = rule_vector[i];
        laser::formula::Formula *formula = rule->get_head();
        std::cout << "THIRD Hello, "
                  << formula->get_predicate() << "!" << std::endl;
    }
    // delete &formula; -> error: because rule deconstructor tries to free it.
}

//==============================================================================


/*
 * This example results in a segmentation fault in make_rule_vector()
 * We create a Rule object in make_rule(). This object is copied to a new object
 * when make_rule returns, and the old object is deleted using ~Rule().
 * ~Rule() deletes the *formula object that is stored in Rule.head, thus the
 *  copy returned by make_rule() has an invalid pointer as head.
 * Proof: changing Rule::~Rule()=default; results in no sigsevg
 * Solution 1 - Pointers: pass Rule* created with 'new'
 * Solution 2 - The rule of three/five/zero:
 * https://en.cppreference.com/w/cpp/language/rule_of_three
 * I need to implement copy constructor and copy assignment operator for
 * Rule and all the objects used in Rule that manage a resource whose handle is
 * an object of non-class type (pointers) -> RuleBody
 */
laser::rule::Rule make_rule() {
    std::string world = "world";
    laser::formula::Atom *local_atom = new laser::formula::Atom(world);
    laser::formula::Formula *local_formula = local_atom;
    laser::rule::Rule local_rule = laser::rule::Rule(local_formula,
            std::vector<laser::formula::Formula*>());

    laser::formula::Formula *formula = local_rule.get_head();
    std::cout << "FIRST Hello, "
              << formula->get_predicate() << "!" << std::endl;

    return local_rule;
}

std::vector<laser::rule::Rule> make_rule_vector(int n) {
    std::vector<laser::rule::Rule> vector;
    for (int i = 0; i < n; i++){
        laser::rule::Rule  rule = make_rule();
        laser::formula::Formula *formula = rule.get_head();
        std::cout << "SECOND Hello, "
                  << formula->get_predicate() << "!" << std::endl;
        vector.push_back(rule);
    }
    return vector;
}

int main() {

    int n = 5;
    auto rule_vector = make_rule_vector(n);
    for (int i = 0; i < n; i++) {
        laser::rule::Rule rule = rule_vector[i];
        laser::formula::Formula *formula = rule.get_head();
        std::cout << "THIRD Hello, "
                  << formula->get_predicate() << "!" << std::endl;
    }
    // delete &formula; -> error: because rule deconstructor tries to free it.
}

// ===========================================================================
/* In this solution we don't need to use 'new' because now Rule and RuleBody
 * will clone each Formula object. Formula has some new methods "clone()" and
 * "move()" which are used by Rule and RuleBody to allocate clone objects on
 * the heap. This way classes can manage their own objects. */

laser::rule::Rule make_rule() {
    std::string world = "world";
    laser::formula::Atom local_atom_hello = laser::formula::Atom("hello");
    laser::formula::Atom local_atom_world = laser::formula::Atom(world);
    laser::rule::Rule local_rule = laser::rule::Rule(local_atom_hello);
    local_rule.add_body_fomrula(local_atom_world);

    laser::formula::Formula &formula_hello = local_rule.get_head();
    laser::formula::Formula &formula_world = local_rule.get_body_formula(0);
    std::cout << "FIRST : " << formula_hello.get_predicate() << " "
              << formula_world.get_predicate() << "!" << std::endl;

    return local_rule;
}

std::vector<laser::rule::Rule> make_rule_vector(int n) {
    std::vector<laser::rule::Rule> vector;
    for (int i = 0; i < n; i++){
        laser::rule::Rule  rule = make_rule();
        laser::formula::Formula &formula = rule.get_head();

        laser::formula::Formula &formula_hello = rule.get_head();
        laser::formula::Formula &formula_world = rule.get_body_formula(0);
        std::cout << "SECOND : " << formula_hello.get_predicate() << " "
                  << formula_world.get_predicate() << "!" << std::endl;

        vector.push_back(rule);
    }
    return vector;
}

int main() {

    int n = 5;
    auto rule_vector = make_rule_vector(n);
    for (int i = 0; i < n; i++) {
        laser::rule::Rule rule = rule_vector[i];

        laser::formula::Formula &formula_hello = rule.get_head();
        laser::formula::Formula &formula_world = rule.get_body_formula(0);
        std::cout << "THIRD : " << formula_hello.get_predicate() << " "
                  << formula_world.get_predicate() << "!" << std::endl;
    }
    // delete &formula; -> error: because rule deconstructor tries to free it.
}