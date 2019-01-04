#include "formula/extended/conjunction.h"

#include <algorithm>

namespace laser {
namespace formula {

Conjunction::~Conjunction() {
    delete left_child;
    delete right_child;
}

Conjunction::Conjunction(Formula *left_child, Formula *right_child) {
    this->left_child = &left_child->move();
    this->right_child = &right_child->move();
    populate_variable_collections();
}

Formula &Conjunction::create() const {
    auto result = new Conjunction();
    return *result;
}

Formula &Conjunction::clone() const {
    auto result = new Conjunction(&this->left_child->clone(),
                                  &this->right_child->clone());
    return *result;
}

Formula &Conjunction::move() {
    auto result =
        new Conjunction(&this->left_child->move(), &this->right_child->move());
    return *result;
}

void Conjunction::set_head(bool is_head) {
    is_head_m = is_head;
}

bool Conjunction::is_head() const {
    return is_head_m;
}

void Conjunction::populate_variable_collections() {
    // Get all variables from children and insert into variable_names
    auto left = left_child->get_variable_names();
    auto right = right_child->get_variable_names();
    variable_names.insert(variable_names.end(), left.begin(), left.end());
    variable_names.insert(variable_names.end(), right.begin(), right.end());
    sort(variable_names.begin(), variable_names.end());
    // Get all duplicates, and insert into common_child variables
    for (int i = 0; i < variable_names.size() - 1; i++) {
        if (variable_names[i] == variable_names[i + 1]) {
            common_child_variables.push_back(variable_names[i]);
        }
    }
    // Remove all duplicates
    variable_names.erase(unique(variable_names.begin(), variable_names.end()),
                         variable_names.end());
    common_child_variables.erase(
        unique(common_child_variables.begin(), common_child_variables.end()),
        common_child_variables.end());
    // populate variable_map
    for (int i = 0; i < variable_names.size(); i++) {
        variable_map.try_emplace(variable_names[i], i);
    }
}
// getters / setters

FormulaType Conjunction::get_type() const { return FormulaType::CONJUNCTION; }

std::vector<std::string> Conjunction::get_predicate_vector() const {
    auto left = left_child->get_predicate_vector();
    auto right = right_child->get_predicate_vector();
    return concatenate_vectors(left, right);
}

// methods

template <typename T, typename A>
std::vector<T, A>
Conjunction::concatenate_vectors(std::vector<T, A> left,
                                 std::vector<T, A> right) const {
    left.insert(left.end(), right.begin(), right.end());
    sort(left.begin(), left.end());
    left.erase(unique(left.begin(), left.end()), left.end());
    return left;
}

std::vector<std::string> Conjunction::get_variable_names() const {
    return variable_names;
}

std::vector<std::string> Conjunction::get_full_variable_names() const {
    // this method is not at all useful for conjunction
    return variable_names;
}

int Conjunction::get_variable_index(std::string variable_name) const {
    int result = -1;
    if (variable_map.count(variable_name) > 0) {
        result = variable_map.at(variable_name);
    }
    return result;
}

bool Conjunction::is_satisfied() const {
    return left_child->is_satisfied() && right_child->is_satisfied();
}

size_t Conjunction::get_number_of_variables() const {
    return variable_names.size();
}

std::string Conjunction::debug_string() const {
    return left_child->debug_string();
    // TODO
}

void Conjunction::add_child(formula::Formula *child) {
    if (!left_child) {
        left_child = &child->move();
    } else if (!right_child) {
        right_child = &child->move();
        populate_variable_collections();
    }
}

void Conjunction::expire_outdated_groundings(util::Timeline timeline) {
    left_child->expire_outdated_groundings(timeline);
    right_child->expire_outdated_groundings(timeline);
    grounding_vector.clear();
}

std::vector<Grounding>
Conjunction::get_groundings(util::Timeline timeline) {
    return grounding_vector;
}

std::vector<Grounding>
Conjunction::get_conclusions_timepoint(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::vector<Grounding>
Conjunction::get_conclusions_step(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::string
Conjunction::hash_common_variables(Formula const &child,
                                   Grounding const &grounding) const {
    if (common_child_variables.empty()) {
        return "0";
    }
    std::stringstream ss;
    for (auto &variable : common_child_variables) {
        size_t variable_index = child.get_variable_index(variable);
        auto constant = grounding.get_constant(variable_index);
        ss << constant << ";;";
    }
    return ss.str();
}

Grounding Conjunction::merge_groundings(util::Timeline timeline,
                                        Grounding const &left,
                                        Grounding const &right) const {
    auto ct = timeline.get_time();
    auto ht = timeline.min(left.get_horizon_time(), right.get_horizon_time());
    // TODO not sure about tuple counter values;
    auto cc = timeline.get_tuple_count();
    auto hc = timeline.min(left.get_horizon_count(), right.get_horizon_count());
    std::vector<std::string> substitution_vector;
    for (auto const &variable_name : variable_names) {
        auto left_index = left_child->get_variable_index(variable_name);
        if (left_index >= 0) {
            substitution_vector.push_back(left.get_constant(left_index));
        } else {
            auto right_index = right_child->get_variable_index(variable_name);
            substitution_vector.push_back(right.get_constant(right_index));
        }
    }
    auto result = Grounding(ct, ht, cc, hc, substitution_vector);
    return result;
}

void Conjunction::populate_grounding_vector(
    util::Timeline timeline, std::vector<Grounding> left_groundings,
    std::vector<Grounding> right_groundings) {
    std::unordered_map<std::string, std::vector<Grounding>> hashmap;
    for (auto const &gr : right_groundings) {
        auto key = hash_common_variables(*right_child, gr);
        hashmap.try_emplace(key);
        std::vector<Grounding> &map_vector = hashmap[key];
        map_vector.push_back(gr);
    }
    for (auto const &gl : left_groundings) {
        auto key = hash_common_variables(*left_child, gl);
        if (hashmap.count(key) > 0) {
            auto hashmap_values = hashmap.at(key);
            for (auto const &gr : hashmap_values) {
                auto current_time = timeline.get_time();
                if (gl.get_consideration_time() == current_time ||
                    gr.get_consideration_time() == current_time) {
                    auto new_grounding = merge_groundings(timeline, gl, gr);
                    grounding_vector.push_back(new_grounding);
                }
            }
        }
    }
}

bool Conjunction::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string, std::vector<formula::Grounding>> const &facts) {
    // TODO Here I can split facts in sub-maps, based on predicates of children
    bool result = left_child->evaluate(timeline, facts);
    result |= right_child->evaluate(timeline, facts);
    populate_grounding_vector(timeline, left_child->get_groundings(timeline),
                              right_child->get_groundings(timeline));
    return !grounding_vector.empty();
}

} // namespace formula
} // namespace laser
