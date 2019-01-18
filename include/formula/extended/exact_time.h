#ifndef LASER_FORMULA_EXTENDED_EXACT_TIME_H
#define LASER_FORMULA_EXTENDED_EXACT_TIME_H

#include <string>
#include <set>

#include "formula/formula.h"
#include "formula/grounding.h"
#include "formula/grounding_table.h"

namespace laser {
namespace formula {

/**
 * ExactTime (@) Formula
 */
class ExactTime : public Formula {
  private:
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
    std::unordered_map<
        uint64_t,
        std::set<std::shared_ptr<Grounding>, GroundingFullCompare>>
        future_conclusion_map;

    std::vector<std::shared_ptr<Grounding>> timepoint_conclusions;

    std::vector<std::shared_ptr<Grounding>> convert_groundings_head(
        util::Timeline timeline,
        std::vector<std::shared_ptr<Grounding>> const &groundings) const;

    /**
     * Adds the Time Variable to all the groundings in groundings vector
     */
    std::vector<std::shared_ptr<Grounding>> convert_groundings_body(
        util::Timeline timeline,
        std::vector<std::shared_ptr<Grounding>> groundings) const;

    /**
     * Removes the Time Variable from all the groundings in groundings vector
     */
    std::vector<std::shared_ptr<Grounding>>
    revert_groundings(util::Timeline timeline,
                      std::vector<std::shared_ptr<Grounding>> groundings) const;

    std::shared_ptr<Grounding>
    add_time_variable(util::Timeline timeline,
                      Grounding const &grounding) const;

    std::shared_ptr<Grounding>
    remove_time_variable(util::Timeline timeline,
                         Grounding const &grounding) const;

    void init();

    size_t get_time_variable_index() const;

    void evaluate_head(
        util::Timeline timeline,
        std::unordered_map<
            std::string, std::vector<std::shared_ptr<Grounding>>> const &facts);

    void evaluate_body(
        util::Timeline timeline,
        std::unordered_map<
            std::string, std::vector<std::shared_ptr<Grounding>>> const &facts);

  public:
    // constructors / destructors

    ExactTime() = default;
    ExactTime(std::string time_variable, Formula *child);
    ~ExactTime() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    // getters / setters

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    // methods

    std::vector<std::string> get_variable_names() const override;

    std::vector<std::string> get_full_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<Grounding>>
    get_groundings(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_step(util::Timeline timeline) override;
    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_timepoint(util::Timeline timeline) override;

    std::string debug_string() const override;

    bool
    evaluate(util::Timeline timeline,
             std::unordered_map<std::string,
                                std::vector<std::shared_ptr<Grounding>>> const
                 &facts) override;

    void expire_outdated_groundings(util::Timeline timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_EXACT_TIME_H
