#include "formula/extended/box.h"

namespace laser {
namespace formula {

Box::~Box() { delete child; }

Box::Box(Formula *child) { this->child = &child->move(); }

Formula &Box::create() const {
    auto result = new Box();
    return *result;
}

Formula &Box::clone() const {
    auto result = new Box(&this->child->clone());
    return *result;
}

Formula &Box::move() {
    auto result = new Box(&this->child->move());
    return *result;
}

// getters / setters

void Box::set_head(bool is_head) { is_head_m = is_head; }

bool Box::is_head() const { return is_head_m; }

FormulaType Box::get_type() const { return FormulaType::BOX; }

std::vector<std::string> Box::get_predicate_vector() const {
    return child->get_predicate_vector();
}

// methods

std::vector<std::string> Box::get_variable_names() const {
    return child->get_variable_names();
}

std::vector<std::string> Box::get_full_variable_names() const {
    return child->get_full_variable_names();
}

int Box::get_variable_index(std::string variable_name) const {
    return child->get_variable_index(variable_name);
}

bool Box::is_satisfied() const { return child->is_satisfied(); }

size_t Box::get_number_of_variables() const {
    return child->get_number_of_variables();
}

std::string Box::debug_string() const { return child->debug_string(); }

void Box::add_child(formula::Formula *child) {}

void Box::expire_outdated_groundings(util::Timeline timeline) {
    child->expire_outdated_groundings(timeline);
    grounding_table.expire_outdated_groundings(timeline.get_time(),
                                               timeline.get_min_tuple_count());
    // Next we remove all groundings in the map that have the horizon
    // values outside the timeline (imposed or not by a window).
    for (auto iterator = box_map.begin(); iterator != box_map.end();
         /*nothing*/) {
        auto const &key = iterator->first;
        auto grounding = iterator->second;
        if (grounding->get_horizon_time() < timeline.get_min_time()) {
            iterator = box_map.erase(iterator);
        } else {
            iterator++;
        }
    }
}

std::vector<std::shared_ptr<Grounding>>
Box::get_groundings(util::Timeline timeline) {
    auto grounding_vector = grounding_table.get_all_groundings();
    return grounding_vector;
}

std::vector<std::shared_ptr<Grounding>>
Box::get_conclusions_timepoint(util::Timeline timeline) {
    return get_groundings(timeline);
}

std::vector<std::shared_ptr<Grounding>>
Box::get_conclusions_step(util::Timeline timeline) {
    return grounding_table.get_recent_groundings();
}

bool Box::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    bool result = child->evaluate(timeline, facts);
    if (result) {
        auto child_facts = child->get_groundings(timeline);
        update_box_map(child_facts);
        auto box_groundings = compute_box_conclusions(timeline);
        grounding_table.add_grounding_vector(box_groundings);
    }
    return grounding_table.has_recent_groundings();
}

std::vector<std::shared_ptr<Grounding>>
Box::compute_box_conclusions(util::Timeline timeline) {
    std::vector<std::shared_ptr<Grounding>> result;
    uint64_t current_time = timeline.get_time();
    uint64_t start_time = timeline.get_min_time();
    uint64_t current_count = timeline.get_tuple_count();
    uint64_t start_count = timeline.get_min_tuple_count();

    for (auto &iterator : box_map) {
        auto const &key = iterator.first;
        auto const &grounding = iterator.second;
        auto ct = grounding->get_consideration_time();
        auto ht = grounding->get_horizon_time();
        auto cc = grounding->get_consideration_count();
        auto hc = grounding->get_horizon_count();

        // TODO also add tuple cunter condition
        if (ct <= start_time && ht >= current_time) {
            auto new_grounding = grounding->new_annotations(
                current_time, current_time, current_count, current_count);
            result.push_back(new_grounding);
        }
    }
    return result;
}

void Box::update_box_map(std::vector<std::shared_ptr<Grounding>> const &facts) {
    bool keep_going = true;
    while (keep_going) {
        // We need to repete as we may have in box p(a)[1, 1], and get
        // from child p(a)[4,4], p(a)[2,3], in this order;
        keep_going = false;
        for (auto const &child_grounding : facts) {
            size_t key = child_grounding->get_hash();
            box_map.try_emplace(key, child_grounding);
            std::shared_ptr<Grounding> &box_grounding = box_map[key];
            auto adjusted_result =
                adjust_annotation(box_grounding, child_grounding);
            keep_going |= adjusted_result.first;
            if (adjusted_result.first) {
                auto map_tuple = box_map.find(key);
                map_tuple->second = adjusted_result.second;
            }
        }
    }
}

std::pair<bool, std::shared_ptr<Grounding>> Box::adjust_annotation(
    std::shared_ptr<Grounding> const &box_grounding,
    std::shared_ptr<Grounding> const &child_grounding) const {
    bool is_modified = false;
    auto ct1 = box_grounding->get_consideration_time();
    auto ht1 = box_grounding->get_horizon_time();
    auto cc1 = box_grounding->get_consideration_count();
    auto hc1 = box_grounding->get_horizon_count();
    auto ct2 = child_grounding->get_consideration_time();
    auto ht2 = child_grounding->get_horizon_time();
    auto cc2 = child_grounding->get_consideration_count();
    auto hc2 = child_grounding->get_horizon_count();

    // Adjust Time annotations:
    if ((ct2 < ct1) && (ht2 >= ct1 - 1)) {
        if (ct2 < ct1) {
            ct1 = ct2;
            is_modified = true;
        }
        if (ht2 > ht1) {
            ht1 = ht2;
            is_modified = true;
        }
    } else if ((ct2 >= ct1) && (ct2 <= ht1 + 1) && (ht2 > ht1)) {
        ht1 = ht2;
        is_modified = true;
    }

    // Adjust Tuple Counter annotations:
    // TODO

    if (is_modified) {
        auto new_box_grounding =
            box_grounding->new_annotations(ct1, ht1, cc1, hc1);
        return {is_modified, new_box_grounding};
    }
    return {is_modified, box_grounding};
}

} // namespace formula
} // namespace laser
