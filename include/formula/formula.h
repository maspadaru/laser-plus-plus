#ifndef LASER_FORMULA_FORMULA_H
#define LASER_FORMULA_FORMULA_H

#include <map>
#include <memory>
#include <string>

#include "formula_type.h"
#include "util/database.h"
#include "util/grounding.h"
#include "util/timeline.h"

namespace laser::formula {

/**
 * Formula Interface
 */
class Formula {
  private:
  public:
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

    virtual void set_head(bool is_head) = 0;

    virtual bool is_head() const = 0;

    virtual uint64_t get_window_size() const = 0;

    virtual FormulaType get_type() const = 0;

    virtual std::vector<std::string> const &get_predicate_vector() const = 0;

    virtual std::map<std::string, size_t> const &get_arity_map() const = 0;

    virtual void add_child(formula::Formula *child) = 0;

    virtual std::vector<formula::Formula *> get_children() const = 0;

    /**
     * Return a list of all variable names, including duplicates.
     * Useful for formating the output of a program.
     * E.g.: p(X, X, Y) -> ["X", "X", "Y"]
     */
    virtual std::vector<std::string> const &get_variable_names() const = 0;

    virtual int get_variable_index(std::string const &variable_name) const = 0;

    virtual bool
    evaluate(util::Timeline const &timeline, size_t previous_step,
             std::vector<std::shared_ptr<util::Grounding>> const &facts) = 0;

    virtual size_t get_number_of_variables() const = 0;

    virtual void expire_outdated_groundings(util::Timeline const &timeline) = 0;

    virtual std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) = 0;

    /**
     * Returns only the new conclusion that were derived since the rule was last
     * evaluated.
     * Useful when adding new conclusions as facts to the input of the next
     * evaluation step during some timepoint.
     * Used to get conclusions from the head of the rule, i.e.: only for
     * Atom and Exact_Time
     */
    virtual std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) = 0;

    /**
     * Returns all the new conclusion that were derived during the current
     * timepoint Useful when writing conclusions to output. Used to get
     * conclusions from the head of the rule, i.e.: only for Atom and Exact_Time
     */
    virtual std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) = 0;
};

} // namespace laser::formula

#endif // LASER_FORMULA_FORMULA_H
