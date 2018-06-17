#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <input/dataparser.h>
#include <output/output.h>
#include <program/program.h>
#include <rule/rule.h>


int main() {

    laser::formula::Atom atom = laser::formula::Atom();
    laser::formula::Formula& formula = (laser::formula::Formula&) atom;
    formula.print_me();
    laser::input::print_me();
    laser::output::print_me();
    laser::program::print_me();
    laser::rule::print_me();
    return 0;
}