#ifndef LASER_FORMULA_EXTENDED_MATH_H
#define LASER_FORMULA_EXTENDED_MATH_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "formula/extended/atom.h"
#include "formula/formula.h"
#include "formula/formula_type.h"
#include "formula/grounding_table.h"
#include "formula/math_operator.h"
#include "util/grounding.h"
#include "util/shared.h"
#include "util/timeline.h"

namespace laser::formula {

class Math : public Formula {
  private:
    FormulaType formula_type = FormulaType::MATH;
    std::string predicate;
    std::string math_sign;
    MathOperator math_operator;
    std::unique_ptr<Formula> child;
    std::vector<std::string> arguments;

    MathOperator generate_operator(std::string const &math_sign) const;

    std::string
    generate_predicate(std::string const &math_sign,
                       std::vector<std::string> const &arguments) const;

    void init();

  public:
    explicit Math(std::string math_sign, std::vector<std::string> arguments);

    ~Math() override = default;

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

#endif // LASER_FORMULA_EXTENDED_MATH_H
