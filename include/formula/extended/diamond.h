#ifndef LASER_FORMULA_EXTENDED_DIAMOND_H
#define LASER_FORMULA_EXTENDED_DIAMOND_H

#include <memory>
#include <string>

#include "formula/formula.h"
#include "formula/grounding_table.h"
#include "util/grounding.h"

namespace laser::formula {

/**
 * Dimond Formula
 */
class Diamond : public Formula {
  private:
    FormulaType formula_type = FormulaType::DIAMOND;
    bool is_head_m = false;
    std::unique_ptr<formula::Formula> child;
    GroundingTable grounding_table;

    std::vector<std::shared_ptr<util::Grounding>> convert_facts(
        std::vector<std::shared_ptr<util::Grounding>> const &grounding_vector,
        util::Timeline const &timeline);

  public:
    Diamond() = default;

    explicit Diamond(std::unique_ptr<formula::Formula> child);

    std::unique_ptr<formula::Formula> clone() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_new_facts(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_old_facts(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions(util::Timeline const &timeline) override;

    void new_step(uint64_t current_time) override;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    void add_child(std::unique_ptr<formula::Formula> child) override;

    std::vector<std::unique_ptr<formula::Formula> const *>
    get_children() const override;

    uint64_t get_window_size() const override;
};

} // namespace laser::formula

#endif // LASER_FORMULA_EXTENDED_DIAMOND_H
