#ifndef LASER_FORMULA_EXTENDED_TIME_WINDOW_H
#define LASER_FORMULA_EXTENDED_TIME_WINDOW_H

#include <string>

#include "formula/formula.h"
#include "formula/grounding.h"

namespace laser {
namespace formula {

/**
 * Time Window Formula
 */
class TimeWindow : public Formula {
  private:
    uint64_t past_size = 0; // L
    uint64_t future_size = 0; // U
    uint64_t step_size = 0; // D
    uint64_t pivot_time = 0;
    Formula* child;

    Grounding adjust_annotations(Grounding grounding) const;

  public:
    // constructors / destructors

    TimeWindow() = default;
    ~TimeWindow() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    // getters / setters

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    // methods

    std::vector<std::string> get_variable_names() const override;

    std::vector<std::string> get_full_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    size_t get_number_of_variables() const override;

    bool
    evaluate(util::Timeline timeline,
             std::unordered_map<std::string, std::vector<formula::Grounding>>
                 facts) override;

    void expire_outdated_groundings(util::Timeline timeline) override;

    std::vector<Grounding> get_groundings() const override;

    std::string debug_string() const override;
    
    void add_child(formula::Formula* child) override;

    // Own methodds, not inherited from Formula interface:

    explicit TimeWindow(uint64_t size, Formula* child);

    explicit TimeWindow(uint64_t past_size, uint64_t future_size, 
            uint64_t step_size, Formula* child);

};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_TIME_WINDOW_H
