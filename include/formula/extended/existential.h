#ifndef LASER_FORMULA_EXTENDED_EXISTENTIAL_H
#define LASER_FORMULA_EXTENDED_EXISTENTIAL_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "formula/formula.h"
#include "util/grounding.h"

namespace laser {
namespace formula {

/**
 * Formula begining with a Existential Quantifier operator
 */
class Existential : public Formula {
  private:
    std::vector<Formula *> children;
    std::vector<std::string> predicate_vector;
    std::vector<std::string> bound_variables;
    std::vector<std::string> free_variables;
    std::vector<std::string> child_variables;
    std::unordered_map<std::string, int> free_variable_index;
    std::unordered_map<std::string, int> bound_variable_index;
    std::unordered_map<std::string, int> child_variable_index;
    std::unordered_map<size_t, std::vector<std::string>> skolem_map;
    uint64_t null_value_count = 0;

    void evaluate_children(
        util::Timeline const &timeline, util::Database const &database,
        std::vector<std::shared_ptr<util::Grounding>> const &facts);

    bool is_valid_sne(util::Timeline const &timeline,
                      std::shared_ptr<util::Grounding> grounding) const;

    std::vector<std::shared_ptr<util::Grounding>> build_chase_facts(
        util::Timeline const &timeline,
        std::vector<std::shared_ptr<util::Grounding>> const &facts);

    std::shared_ptr<util::Grounding>
    generate_chase_fact(std::shared_ptr<util::Grounding> const &body_grounding);

    std::shared_ptr<util::Grounding>
    make_child_fact(std::shared_ptr<util::Grounding> const &skolem_fact,
                    Formula *child) const;

    std::string generate_new_value(std::string const &var_name);

    void init_variable_vectors();

    std::unordered_map<std::string, int>
    make_index(std::vector<std::string> const &vector);

  public:
    explicit Existential(std::vector<Formula *> children);
    Existential() = default;
    ~Existential() override;

    Formula &create() const override;
    Formula &clone() const override;
    Formula &move() override;

    void set_head(bool is_head) override;

    bool is_head() const override;

    FormulaType get_type() const override;

    std::vector<std::string> get_predicate_vector() const override;

    std::vector<std::string> const &get_variable_names() const override;

    int get_variable_index(std::string const &variable_name) const override;

    size_t get_number_of_variables() const override;

    bool evaluate(
        util::Timeline const &timeline, util::Database const &database,
        std::vector<std::shared_ptr<util::Grounding>> const &facts) override;

    void expire_outdated_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_groundings(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_timepoint(util::Timeline const &timeline) override;

    std::vector<std::shared_ptr<util::Grounding>>
    get_conclusions_step(util::Timeline const &timeline) override;

    void add_child(formula::Formula *child) override;
};

} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_EXTENDED_EXISTENTIAL_H
