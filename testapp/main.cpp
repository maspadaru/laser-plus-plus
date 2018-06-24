#include <iostream>

#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <rule/rule.h>

#include <simple_reader.h>
#include <simple_parser.h>
#include <input/input_manager.h>
#include <program/program.h>
#include <simple_formatter.h>
#include <simple_writer.h>

std::vector<laser::rule::Rule> make_rule_vector(int n);

int main() {

    // ========== First test: memory management =========
    int n = 5;
    auto rule_vector = make_rule_vector(n);
    for (int i = 0; i < n; i++) {
        laser::rule::Rule rule = rule_vector[i]; // Copy constructor is called

        laser::formula::Formula &formula_hello = rule.get_head();
        laser::formula::Formula &formula_world = rule.get_body_formula(0);
        std::cerr << "THIRD : " << formula_hello.get_predicate() << " "
                  << formula_world.get_predicate() << "!" << std::endl;
    } // rule goes out of scope and DELETE is called on it <- at each iteration

    std::cerr << "===============================================" << std::endl;
    std::cerr << std::endl << std::endl << std::endl  << std::endl  << std::endl;

    // =========== Now test Laser =============

    std::string stream_string =
            "1 14"
            "1 : \n"
            "2 : c(x4, z)\n"
            "2 : d(x5, z)\n"
            "4 : \n"
            "5 : \n"
            "5 : e(x7, y), e(z, x7), e(x9, y)\n"
            "5 : e(x1, y)"
            "6 : a(x1, y)\n"
            "7 : \n"
            "8 : a(x2, y), b(y, z)\n"
            "9 : \n"
            "10 : a(x3, y)\n"
            "11 : \n"
            "12 : \n"
            "13 : \n"
            "14 : \n";

    std::string rule_string =
            "r1 : q(X, Y, Z) :- a(X, Y) and b(Y, Z)\n"
            "r2 : r(X, Y) :- c(X, Y)\n"
            "r3 : s(Z, Y) :- e(X, Y) and e(Z, X)\n"
            "r4 : t(X, Y) :- a(X, Y)\n";

    auto simple_data_reader = SimpleReader();
    simple_data_reader.set_source(stream_string);
    auto simple_rule_reader = SimpleReader();
    simple_rule_reader.set_source(rule_string);
    auto simple_parser = SimpleParser();
    auto input_manager = laser::input::InputManager();
    input_manager.initialize_rule_reader(&simple_rule_reader, &simple_parser);
    input_manager.initialize_stream_reader(&simple_data_reader, &simple_parser);

    auto simple_formatter = SimpleFormatter();
    auto simple_writer = SimpleWriter();
    auto output_manager = laser::output::OutputManager();
    output_manager.initialize_output_manager(&simple_writer, &simple_formatter);

    auto program = laser::program::Program(input_manager, output_manager);

    while (!program.is_done()) {
        program.evaluate();
    }






    std::cerr << "Hello, " << "!" << std::endl;
} // calls Delete on rule_vector, program, simple_writer, simple_formatter,
// simple_data_reader, simple_rule_reader, simple_parser, rule_string,
// stream_string


// ======= The following functions are used to test memory management =====

laser::rule::Rule make_rule() {
    std::string world = "world";
    laser::formula::Atom local_atom_hello = laser::formula::Atom("hello");
    laser::formula::Atom local_atom_world = laser::formula::Atom(world);
    std::vector<laser::formula::Formula*> body_formulas;
    body_formulas.push_back(&local_atom_world);
    laser::rule::Rule local_rule = laser::rule::Rule(&local_atom_hello, body_formulas);

    laser::formula::Formula &formula_hello = local_rule.get_head();
    laser::formula::Formula &formula_world = local_rule.get_body_formula(0);
    std::cerr << "FIRST : " << formula_hello.get_predicate() << " "
              << formula_world.get_predicate() << "!" << std::endl;

    return local_rule;
// calls Delete on local_atom_hello, local_atom_world, body_formulas
// no move/copy/delete is called on local_rule <- most likely the compiler
//    optimizes to return by reference
}

std::vector<laser::rule::Rule> make_rule_vector(int n) {
    std::vector<laser::rule::Rule> vector;
    for (int i = 0; i < n; i++){
        laser::rule::Rule  rule = make_rule();
        laser::formula::Formula &formula_hello = rule.get_head();
        laser::formula::Formula &formula_world = rule.get_body_formula(0);
        std::cerr << "SECOND : " << formula_hello.get_predicate() << " "
                  << formula_world.get_predicate() << "!" << std::endl;

        vector.push_back(rule);
        /* vector:
         * On push_back: - Copy constructor called on rule before push_back,
         * On reallocate: - Copy const called on rule into the reallocated vector
         *                - Move constructor is called on all elements in old vec
         *                - delete called on all elements of the old vector
         */
    } // rule goes out of scope and DELETE is called on it <- at each iteration
    return vector;
} // No move/copy/delete gets called on rules when Vector is returned. Ret ref?


/* *******  notes: *******************
 * atoms in RuleBody are never deleted on a Move -> move of vector is very efficient
 *
 * When a Rule is cloned:
 *      - Atom.clone() is called on the head
 *      - RuleBody(RuleBody const &other) is called on body
 *              - in turn this calls Atom.clone() on each atom in the body
 *
 * When a Rule is moved:
 *      - Atom.move() is moved to the head of the rule
 *      - RuleBody(std::vector<formula::Formula *> parameter_vector) is called
 *        but needs to be optimized. See rule_body.cpp
 *
 * When Rule constructor Rule(head, vector<> body) is called:
 *      - Atom.move() is moved to the head of the rule
 *      - RuleBody(std::vector<formula::Formula *> parameter_vector) is called
 *        but needs to be optimized. See rule_body.cpp
 *
 * When a Rule is deleted using ~Rule() or when it goes out of scope:
 *      - the head atom is deleted using ~Atom()
 *      - The body is deleted using ~RuleBody(), in turn calling delete on each
 *        atom in the body.
 *
 * Every time a rule is passed as a parameter, for instance, when added to a
 * vector, the rule is Cloned. Optimization: use std::move(rule) for parameters
 * and references in loops.
 *
 * When used in a for loop for (auto rule: rule_vector), the rule gets copied
 * from the vector, and gets deleted at the end of the loop. Optimization:
 * A reference should be used: [auto const &rule]
 *
 * When a Rule is returned from a function it does not get Cloned, Moved or
 * deleted. I assume this is due to a compiler optimization where it
 * automatically returns a reference.
 *
 * When a vector of Rules is reallocated, it Moves the rules into the new
 * allocation.
 *
 * Any object declared on the stack is freed using 'delete' when it goes out
 * of scope.
 *
 */

/*
 * ================== Sample output: =======================================
 * /home/mike/Workspace/LaserPP/cmake-build-debug/LaserPP
CREATE Atom -> predicate:  hello
CREATE Atom -> predicate:  world
Atom.move() -> CREATE Atom -> predicate:  hello
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  world
Rule-CREATE -> head: hello; body size = 1
Body predicates: world, // Rule
FIRST : hello world!
DELETE Atom -> predicate:  world
DELETE Atom -> predicate:  hello
SECOND : hello world!
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
CREATE Atom -> predicate:  hello
CREATE Atom -> predicate:  world
Atom.move() -> CREATE Atom -> predicate:  hello
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  world
Rule-CREATE -> head: hello; body size = 1
Body predicates: world, // Rule
FIRST : hello world!
DELETE Atom -> predicate:  world
DELETE Atom -> predicate:  hello
SECOND : hello world!
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
CREATE Atom -> predicate:  hello
CREATE Atom -> predicate:  world
Atom.move() -> CREATE Atom -> predicate:  hello
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  world
Rule-CREATE -> head: hello; body size = 1
Body predicates: world, // Rule
FIRST : hello world!
DELETE Atom -> predicate:  world
DELETE Atom -> predicate:  hello
SECOND : hello world!
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
CREATE Atom -> predicate:  hello
CREATE Atom -> predicate:  world
Atom.move() -> CREATE Atom -> predicate:  hello
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  world
Rule-CREATE -> head: hello; body size = 1
Body predicates: world, // Rule
FIRST : hello world!
DELETE Atom -> predicate:  world
DELETE Atom -> predicate:  hello
SECOND : hello world!
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
CREATE Atom -> predicate:  hello
CREATE Atom -> predicate:  world
Atom.move() -> CREATE Atom -> predicate:  hello
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  world
Rule-CREATE -> head: hello; body size = 1
Body predicates: world, // Rule
FIRST : hello world!
DELETE Atom -> predicate:  world
DELETE Atom -> predicate:  hello
SECOND : hello world!
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  hello
[RuleBody move] Rule-MOVE -> head: hello; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
THIRD : hello world!
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
THIRD : hello world!
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
THIRD : hello world!
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
THIRD : hello world!
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
Atom.clone() -> hello
[RuleBody clone] Atom.clone() -> world
Rule-COPY -> head: hello; body size = 1
// Rule
THIRD : hello world!
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
===============================================








===== CALL: InputManager->get_rules() =====


=== CALL: SimpleParser->parse_rule() ==

Rule String:  q(X, Y, Z) :- a(X, Y) and b(Y, Z)

CREATE Atom + var_names -> predicate:  q
variable_names.size=: 3; Names: X, Y, Z, DONE;
CREATE Atom + var_names -> predicate:  a
variable_names.size=: 2; Names: X, Y, DONE;
CREATE Atom + var_names -> predicate:  b
variable_names.size=: 2; Names: Y, Z, DONE;




Body predicates: b, a,
Atom.move() -> CREATE Atom -> predicate:  q
size of parameter vector for body: 2
Atom.move() -> CREATE Atom -> predicate:  b
Atom.move() -> CREATE Atom -> predicate:  a
Rule-CREATE -> head: q; body size = 2
Body predicates: b, a, // Rule
Before FREE Body.size = 2; predicates: b, a,

Before FREE. In RULE Body.size = 2; predicates: b, a, // Rule

DELETE Atom -> predicate:  q
DELETE Atom -> predicate:  b
DELETE Atom -> predicate:  a
After FREE Body.size = 0; predicates:

After FREE. In RULE Body.size = 2; predicates: b, a, // Rule



== DONE: parse_rule() ==

Atom.clone() -> q
[RuleBody clone] Atom.clone() -> b
Atom.clone() -> a
Rule-COPY -> head: q; body size = 2
// Rule
~Rule() -> DELETE Atom -> predicate:  q
// Rule
RuleBody delete:
DELETE Atom -> predicate:  b
DELETE Atom -> predicate:  a
// RuleBody

=== CALL: SimpleParser->parse_rule() ==

Rule String:  r(X, Y) :- c(X, Y)

CREATE Atom + var_names -> predicate:  r
variable_names.size=: 2; Names: X, Y, DONE;
CREATE Atom + var_names -> predicate:  c
variable_names.size=: 2; Names: X, Y, DONE;




Body predicates: c,
Atom.move() -> CREATE Atom -> predicate:  r
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  c
Rule-CREATE -> head: r; body size = 1
Body predicates: c, // Rule
Before FREE Body.size = 1; predicates: c,

Before FREE. In RULE Body.size = 1; predicates: c, // Rule

DELETE Atom -> predicate:  r
DELETE Atom -> predicate:  c
After FREE Body.size = 0; predicates:

After FREE. In RULE Body.size = 1; predicates: c, // Rule



== DONE: parse_rule() ==

Atom.clone() -> r
[RuleBody clone] Atom.clone() -> c
Rule-COPY -> head: r; body size = 1
// Rule
Atom.move() -> CREATE Atom -> predicate:  q
[RuleBody move] Rule-MOVE -> head: q; body size = 2
// Rule
~Rule() -> DELETE Atom -> predicate:  q
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  r
// Rule
RuleBody delete:
DELETE Atom -> predicate:  c
// RuleBody

=== CALL: SimpleParser->parse_rule() ==

Rule String:  s(Z, Y) :- e(X, Y) and e(Z, X)

CREATE Atom + var_names -> predicate:  s
variable_names.size=: 2; Names: Z, Y, DONE;
CREATE Atom + var_names -> predicate:  e
variable_names.size=: 2; Names: X, Y, DONE;
CREATE Atom + var_names -> predicate:  e
variable_names.size=: 2; Names: Z, X, DONE;




Body predicates: e, e,
Atom.move() -> CREATE Atom -> predicate:  s
size of parameter vector for body: 2
Atom.move() -> CREATE Atom -> predicate:  e
Atom.move() -> CREATE Atom -> predicate:  e
Rule-CREATE -> head: s; body size = 2
Body predicates: e, e, // Rule
Before FREE Body.size = 2; predicates: e, e,

Before FREE. In RULE Body.size = 2; predicates: e, e, // Rule

DELETE Atom -> predicate:  s
DELETE Atom -> predicate:  e
DELETE Atom -> predicate:  e
After FREE Body.size = 0; predicates:

After FREE. In RULE Body.size = 2; predicates: e, e, // Rule



== DONE: parse_rule() ==

Atom.clone() -> s
[RuleBody clone] Atom.clone() -> e
Atom.clone() -> e
Rule-COPY -> head: s; body size = 2
// Rule
Atom.move() -> CREATE Atom -> predicate:  q
[RuleBody move] Rule-MOVE -> head: q; body size = 2
// Rule
Atom.move() -> CREATE Atom -> predicate:  r
[RuleBody move] Rule-MOVE -> head: r; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  q
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  r
// Rule
RuleBody delete:
// RuleBody
~Rule() -> DELETE Atom -> predicate:  s
// Rule
RuleBody delete:
DELETE Atom -> predicate:  e
DELETE Atom -> predicate:  e
// RuleBody

=== CALL: SimpleParser->parse_rule() ==

Rule String:  t(X, Y) :- a(X, Y)

CREATE Atom + var_names -> predicate:  t
variable_names.size=: 2; Names: X, Y, DONE;
CREATE Atom + var_names -> predicate:  a
variable_names.size=: 2; Names: X, Y, DONE;




Body predicates: a,
Atom.move() -> CREATE Atom -> predicate:  t
size of parameter vector for body: 1
Atom.move() -> CREATE Atom -> predicate:  a
Rule-CREATE -> head: t; body size = 1
Body predicates: a, // Rule
Before FREE Body.size = 1; predicates: a,

Before FREE. In RULE Body.size = 1; predicates: a, // Rule

DELETE Atom -> predicate:  t
DELETE Atom -> predicate:  a
After FREE Body.size = 0; predicates:

After FREE. In RULE Body.size = 1; predicates: a, // Rule



== DONE: parse_rule() ==

Atom.clone() -> t
[RuleBody clone] Atom.clone() -> a
Rule-COPY -> head: t; body size = 1
// Rule
~Rule() -> DELETE Atom -> predicate:  t
// Rule
RuleBody delete:
DELETE Atom -> predicate:  a
// RuleBody
======== DONE Parser. Returning rule_vector ======
===== DONE: Program->get_rules() =====




===== CALL: Program->init_rules() =====

Atom.clone() -> q
[RuleBody clone] Atom.clone() -> b
Atom.clone() -> a
Rule-COPY -> head: q; body size = 2
// Rule
Atom.clone() -> r
[RuleBody clone] Atom.clone() -> c
Rule-COPY -> head: r; body size = 1
// Rule
Atom.clone() -> s
[RuleBody clone] Atom.clone() -> e
Atom.clone() -> e
Rule-COPY -> head: s; body size = 2
// Rule
Atom.clone() -> t
[RuleBody clone] Atom.clone() -> a
Rule-COPY -> head: t; body size = 1
// Rule



init_rules()


Atom.clone() -> q
[RuleBody clone] Atom.clone() -> b
Atom.clone() -> a
Rule-COPY -> head: q; body size = 2
// Rule
init_rules(): before local_rule = new Rule

Atom.clone() -> q
[RuleBody clone] Atom.clone() -> b
Atom.clone() -> a
Rule-COPY -> head: q; body size = 2
// Rule
init_rules(): after local_rule = new Rule
init_rules(): after push_back(local_rule)

~Rule() -> DELETE Atom -> predicate:  q
// Rule
RuleBody delete:
DELETE Atom -> predicate:  b
DELETE Atom -> predicate:  a
// RuleBody
Atom.clone() -> r
[RuleBody clone] Atom.clone() -> c
Rule-COPY -> head: r; body size = 1
// Rule
init_rules(): before local_rule = new Rule

Atom.clone() -> r
[RuleBody clone] Atom.clone() -> c
Rule-COPY -> head: r; body size = 1
// Rule
init_rules(): after local_rule = new Rule
init_rules(): after push_back(local_rule)

~Rule() -> DELETE Atom -> predicate:  r
// Rule
RuleBody delete:
DELETE Atom -> predicate:  c
// RuleBody
Atom.clone() -> s
[RuleBody clone] Atom.clone() -> e
Atom.clone() -> e
Rule-COPY -> head: s; body size = 2
// Rule
init_rules(): before local_rule = new Rule

Atom.clone() -> s
[RuleBody clone] Atom.clone() -> e
Atom.clone() -> e
Rule-COPY -> head: s; body size = 2
// Rule
init_rules(): after local_rule = new Rule
init_rules(): after push_back(local_rule)

~Rule() -> DELETE Atom -> predicate:  s
// Rule
RuleBody delete:
DELETE Atom -> predicate:  e
DELETE Atom -> predicate:  e
// RuleBody
Atom.clone() -> t
[RuleBody clone] Atom.clone() -> a
Rule-COPY -> head: t; body size = 1
// Rule
init_rules(): before local_rule = new Rule

Atom.clone() -> t
[RuleBody clone] Atom.clone() -> a
Rule-COPY -> head: t; body size = 1
// Rule
init_rules(): after local_rule = new Rule
init_rules(): after push_back(local_rule)

~Rule() -> DELETE Atom -> predicate:  t
// Rule
RuleBody delete:
DELETE Atom -> predicate:  a
// RuleBody



init_rules(): before returning
~Rule() -> DELETE Atom -> predicate:  q
// Rule
RuleBody delete:
DELETE Atom -> predicate:  b
DELETE Atom -> predicate:  a
// RuleBody
~Rule() -> DELETE Atom -> predicate:  r
// Rule
RuleBody delete:
DELETE Atom -> predicate:  c
// RuleBody
~Rule() -> DELETE Atom -> predicate:  s
// Rule
RuleBody delete:
DELETE Atom -> predicate:  e
DELETE Atom -> predicate:  e
// RuleBody
~Rule() -> DELETE Atom -> predicate:  t
// Rule
RuleBody delete:
DELETE Atom -> predicate:  a
// RuleBody
===== DONE: Program->init_rules() =====







~Rule() -> DELETE Atom -> predicate:  q
// Rule
RuleBody delete:
DELETE Atom -> predicate:  b
DELETE Atom -> predicate:  a
// RuleBody
~Rule() -> DELETE Atom -> predicate:  r
// Rule
RuleBody delete:
DELETE Atom -> predicate:  c
// RuleBody
~Rule() -> DELETE Atom -> predicate:  s
// Rule
RuleBody delete:
DELETE Atom -> predicate:  e
DELETE Atom -> predicate:  e
// RuleBody
~Rule() -> DELETE Atom -> predicate:  t
// Rule
RuleBody delete:
DELETE Atom -> predicate:  a
// RuleBody
Hello, !
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody
~Rule() -> DELETE Atom -> predicate:  hello
// Rule
RuleBody delete:
DELETE Atom -> predicate:  world
// RuleBody

Process finished with exit code 0

 */