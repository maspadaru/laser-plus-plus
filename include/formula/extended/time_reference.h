#ifndef LASER_FORMULA_EXTENDED_TIME_REFERENCE_H
#define LASER_FORMULA_EXTENDED_TIME_REFERENCE_H

#include <set>
#include <string>

#include "formula/formula.h"
#include "formula/grounding_table.h"
#include "util/grounding.h"

namespace laser::formula {

/**
 * Time Reference (@) Formula
 */
class TimeReference : public Formula {
  private:
    FormulaType formula_type = FormulaType::TIME_REFERENCE;
    Formula *child;
    GroundingTable grounding_table;
    /** Name of the time variable. Will always be on the last possition in the
     * grounding.
     */
    std::string time_variable;

    /**
     * Holds all the groundings returned by
     * grounding_table.get_recent_groundings, that were not yet reported as new
     * conclusions due to the time_variable pointing to some time in the future.
     *      key: time_variable (some timepoint in the future)
     *      value: Set of all groundings sharing the same time variable
     */
    std::unordered_map<uint64_t, std::set<std::shared_ptr<util::Grounding>,
                                          util::GroundingFullCompare>>
        future_conclusion_map;

    std::vector<std::shared_ptr<util::Grounding>> timepoint_conclusions;

    std::vector<std::shared_ptr<util::Grounding>> convert_groundings_head(
        std::vector<std::shared_ptr<util::Grounding>> const &groundings) const;

    /**
     * Adds the Time Variable to all the groundings in groundings vector
     */
    std::vector<std::shared_ptr<util::Grounding>> convert_groundings_body(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> groundings) const;

    /**
     * Removes the Time Variable from all the groundings in groundings vector
     */
    std::vector<std::shared_ptr<util::Grounding>> revert_groundings(
        uint64_t now,
        std::vector<std::shared_ptr<util::Grounding>> groundings) const;

    std::shared_ptr<util::Grounding>
    add_time_variable(util::Timeline const &timeline,
                      util::Grounding const &grounding) const;

    std::shared_ptr<util::Grounding>
    remove_time_variable(util::Grounding const &grounding) const;

    void init();

    size_t get_time_variable_index() const;

    void
    evaluate_head(util::Timeline const &timeline, size_t previous_step,
                  std::vector<std::shared_ptr<util::Grounding>> const &facts);

    void
    evaluate_body(util::Timeline const &timeline, size_t previous_step,
                  std::vector<std::shared_ptr<util::Grounding>> const &facts);

  public:
    TimeReference() = default;
    TimeReference(std::string time_variable, Formula *child);
    ~TimeReference() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    FormulaType get_type() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;
    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;

    std::vector<formula::Formula *> get_children() const override;

    uint64_t get_window_size() const override;
};

} // namespace laser::formula

#endif // LASER_FORMULA_EXTENDED_TIME_REFERENCE_H
