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
