#ifndef LASER_FORMULA_EXTENDED_DIAMOND_H
#define LASER_FORMULA_EXTENDED_DIAMOND_H

#include <memory>
#include <string>

#include "formula/formula.h"
#include "formula/grounding.h"

namespace laser {
namespace formula {

/**
 * Dimond Formula
 */
class Diamond : public Formula {
  private:
    bool is_head_m = false;
    Formula *child;
    std::unordered_map<uint64_t, std::vector<std::shared_ptr<Grounding>>>
        grounding_map;
    std::vector<std::shared_ptr<Grounding>> recent_groundings_vector;

    void add_grounding(std::shared_ptr<Grounding> const &grounding);
    void add_grounding_vector(
        std::vector<std::shared_ptr<Grounding>> const &grounding_vector);
    void expire_from_map(uint64_t expiration_time,
                         uint64_t expiration_tuple_counter);

  public:
    // constructors / destructors

    Diamond() = default;
    explicit Diamond(Formula *child);
    ~Diamond() override;

    Formula &create() const override;
    Formula &clone() const override;

    Formula &move() override;

    // getters / setters

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    // methods

    std::vector<std::string> get_variable_names() const override;

    std::vector<std::string> get_full_variable_names() const override;

    int get_variable_index(std::string variable_name) const override;

    bool is_satisfied() const override;

    size_t get_number_of_variables() const override;

    std::vector<std::shared_ptr<Grounding>>
    get_groundings(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_timepoint(util::Timeline timeline) override;

    std::vector<std::shared_ptr<Grounding>>
    get_conclusions_step(util::Timeline timeline) override;

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

#endif // LASER_FORMULA_EXTENDED_DIAMOND_H
