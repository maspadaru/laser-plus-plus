#ifndef LASER_FORMULA_MATH_ASSIGNMENT_H
#define LASER_FORMULA_MATH_ASSIGNMENT_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "util/grounding.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::formula {

class Assignment : public Formula {
  private:
    FormulaType type = FormulaType::ASSIGNMENT;
    std::string variable;
    std::string value;
    std::vector<std::string> predicate_vector;
    std::map<std::string, size_t> arity_map;
    GroundingTable grounding_table;

    void init();

  public:
    explicit Assignment(std::string variable, std::string value);

    ~Assignment() override = default;

    std::unique_ptr<formula::Formula> clone() const override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    uint64_t get_window_size() const override;

    FormulaType get_type() const override;

    std::vector<std::string> const &get_predicate_vector() const override;

    std::map<std::string, size_t> const &get_arity_map() const override;

    void add_child(std::unique_ptr<formula::Formula> child) override;

    std::vector<std::unique_ptr<formula::Formula> const *>
    get_children() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    bool evaluate(
        util::Timeline const &timeline, size_t previous_step,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    size_t get_number_of_variables() const override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;
};

} // namespace laser::formula

#endif // LASER_FORMULA_MATH_ASSIGNMENT_H
