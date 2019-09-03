#include "formula/extended/box.h"

namespace laser::formula {

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

void Box::set_head(bool is_head) { is_head_m = is_head; }

bool Box::is_head() const { return is_head_m; }

FormulaType Box::get_type() const { return FormulaType::BOX; }

std::vector<std::string> const &Box::get_predicate_vector() const {
    return child->get_predicate_vector();
}

std::map<std::string, size_t> const &Box::get_arity_map() const {
    return child->get_arity_map();
}

std::vector<std::string> const &Box::get_variable_names() const {
    return child->get_variable_names();
}

int Box::get_variable_index(std::string const &variable_name) const {
    return child->get_variable_index(variable_name);
}

size_t Box::get_number_of_variables() const {
    return child->get_number_of_variables();
}

void Box::add_child(formula::Formula *child) {}

void Box::expire_outdated_groundings(util::Timeline const &timeline) {
    child->expire_outdated_groundings(timeline);
    auto time = timeline.get_time();
    auto tuple_count = timeline.get_tuple_count_at(time);
    grounding_table.expire_outdated_groundings(time, tuple_count);
    // Next we remove all groundings in the map that have the horizon
    // values outside the timeline (imposed or not by a window).
    for (auto iterator = box_map.begin(); iterator != box_map.end();
         /*nothing*/) {
        auto const &key = iterator->first;
        auto const &grounding = iterator->second;
        bool is_expired =
            grounding->get_horizon_time() < timeline.get_min_time() ||
            grounding->get_horizon_count() <= tuple_count;
        if (is_expired) {
            iterator = box_map.erase(iterator);
        } else {
            iterator++;
        }
    }
}

std::vector<std::shared_ptr<util::Grounding>>
Box::get_groundings(util::Timeline const &timeline) {
    auto grounding_vector = grounding_table.get_all_groundings();
    return grounding_vector;
}

std::vector<std::shared_ptr<util::Grounding>>
Box::get_conclusions_timepoint(util::Timeline const &timeline) {
    auto result = get_groundings(timeline);
    return result;
}

std::vector<std::shared_ptr<util::Grounding>>
Box::get_conclusions_step(util::Timeline const &timeline) {
    auto result = grounding_table.get_recent_groundings();
    return result;
}

bool Box::evaluate(util::Timeline const &timeline, size_t previous_step,
                   std::vector<std::shared_ptr<util::Grounding>> const &facts) {
    bool result = child->evaluate(timeline, previous_step, facts);
    if (result) {
        auto child_facts = child->get_groundings(timeline);
        update_box_map(child_facts, timeline);
        auto box_groundings = compute_box_conclusions(timeline);
        grounding_table.add_grounding_vector(box_groundings);
    }
    return grounding_table.has_recent_groundings();
}

std::vector<std::shared_ptr<util::Grounding>>
Box::compute_box_conclusions(util::Timeline const &timeline) {
    std::vector<std::shared_ptr<util::Grounding>> result;
    uint64_t current_time = timeline.get_time();
    uint64_t start_time = timeline.get_min_time();

    for (auto &iterator : box_map) {
        auto const &key = iterator.first;
        auto const &grounding = iterator.second;
        auto ct = grounding->get_consideration_time();
        auto ht = grounding->get_horizon_time();
        auto cc = grounding->get_consideration_count();
        auto hc = grounding->get_horizon_count();

        // One grounding should exist at each timepoint in the timeline
        if (ct <= start_time && ht >= current_time) {
            auto new_grounding = grounding->shallow_clone();
            new_grounding->set_annotations(current_time, current_time, cc, hc);
            result.push_back(std::move(new_grounding));
        }
    }
    return result;
}

void Box::update_box_map(
    std::vector<std::shared_ptr<util::Grounding>> const &facts,
    util::Timeline const &timeline) {
    bool keep_going = true;
    while (keep_going) {
        // We need to repete as we may have in box p(a)[1, 1], and get
        // from child p(a)[4,4], p(a)[2,3], in this order;
        keep_going = false;
        for (auto const &child_grounding : facts) {
            auto const key = child_grounding->get_substitution_key();
            box_map.try_emplace(key, child_grounding);
            std::shared_ptr<util::Grounding> &box_grounding = box_map[key];
            auto adjusted_result =
                adjust_annotation(box_grounding, child_grounding, timeline);
            keep_going |= adjusted_result.first;
            if (adjusted_result.first) {
                auto map_tuple = box_map.find(key);
                map_tuple->second = adjusted_result.second;
            }
        }
    }
}

std::pair<bool, std::shared_ptr<util::Grounding>>
Box::adjust_annotation(std::shared_ptr<util::Grounding> const &box_grounding,
                       std::shared_ptr<util::Grounding> const &child_grounding,
                       util::Timeline const &timeline) const {
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
    auto hc = timeline.max(hc1, hc2);
    if (hc != hc1) {
        is_modified = true;
    }

    if (is_modified) {
        auto new_box_grounding = box_grounding->shallow_clone();
        new_box_grounding->set_annotations(ct1, ht1, cc1, hc);
        return {is_modified, new_box_grounding};
    }
    return {is_modified, box_grounding};
}

std::vector<formula::Formula *> Box::get_children() const {
    std::vector<formula::Formula *> result;
    result.push_back(child);
    return result;
} 

uint64_t Box::get_window_size() const {
    return 0;
} 

} // namespace laser::formula
