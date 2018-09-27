//
// Created by mike on 6/16/18.
//

#ifndef LASER_FORMULA_FORMULA_H
#define LASER_FORMULA_FORMULA_H

#include <string>

#include "formula/grounding_table.h"
#include "formula_type.h"
#include "util/timeline.h"

namespace laser {
namespace formula {

/**
 * Formula Interface
 */
class Formula {
  private:
  public:
    // constructors / destructors

    virtual ~Formula() = default;

    virtual Formula &create() const = 0; // Virtual constructor (creation)
    virtual Formula &clone() const = 0;  // Virtual constructor (copying)

    /**
     * Virtual constructor (moving)
     *  All the contents of this object are moved to a new object allocated on
     *  the heap using 'new'.
     * @return reference to the newly allocated object.
     */
    virtual Formula &move() = 0;

    // getters / setters

    virtual FormulaType get_type() const = 0;

    virtual std::vector<std::string> get_predicate_vector() const = 0;

    // methods

    virtual void add_child(formula::Formula* child) = 0;

    /**
     * Return a list of unique variable names
     * E.g.: p(X, X, Y) -> ["X", "Y"]
     */
    virtual std::vector<std::string> get_variable_names() const = 0;

    /**
     * Return a list of all variable names, including duplicates.
     * Useful for formating the output of a program.
     * E.g.: p(X, X, Y) -> ["X", "X", "Y"]
     */
    virtual std::vector<std::string> get_full_variable_names() const = 0;

    virtual int get_variable_index(std::string variable_name) const = 0;

    virtual bool is_satisfied() const = 0;

    virtual bool
    evaluate(util::Timeline timeline,
             std::unordered_map<std::string, std::vector<formula::Grounding>>
                 facts) = 0;

    virtual size_t get_number_of_variables() const = 0;

    virtual void expire_outdated_groundings(util::Timeline timeline) = 0;

    virtual std::vector<Grounding> get_groundings() const = 0;

    virtual std::string debug_string() const = 0;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_H
