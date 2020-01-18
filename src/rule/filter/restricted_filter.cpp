#include "rule/filter/restricted_filter.h"

namespace laser::rule {

void RestrictedFilter::init(
    std::vector<std::unique_ptr<formula::Formula>> const &head_atoms,
    std::vector<std::string> const &head_variables,
    std::vector<std::string> const &free_variables,
    std::vector<std::string> const &bound_variables,
    std::vector<bool> const &is_event_variable,
    std::vector<std::string> const &frontier_variables,
    bool has_event_variables) {
    // TODO First try to sort head_atoms by highest number of frontier variables
    this->head_atom_count = head_atoms.size();
    this->head_variables_count = head_variables.size();
    this->head_variables = head_variables;
    this->free_variables = free_variables;
    this->bound_variables = bound_variables;
    this->frontier_variables = frontier_variables;
    this->free_variable_index = rule::shared::make_index(free_variables);
    this->bound_variable_index = rule::shared::make_index(bound_variables);
    this->head_variable_index = rule::shared::make_index(head_variables);
    this->has_event_variables = has_event_variables;
    this->is_event_variable = is_event_variable;
    init_head_atoms_var_index_atom_to_head(head_atoms);
    init_head_atom_predicates(head_atoms);
    init_body_head_var_index();
    init_event_variable_list();
    use_global_nulls = util::Settings::get_instance().has_global_null_values();
}

void RestrictedFilter::update(util::Timeline const &timeline,
                              size_t previous_step, util::Database &database) {
    if (!has_database_facts) {
        init_database_facts(database);
    }
    auto new_time = timeline.get_time();
    current_timepoint_substitutions.insert(
        current_timepoint_substitutions.end(),
        current_step_substitutions.begin(), current_step_substitutions.end());
    current_step_substitutions.clear();
    current_step_facts.clear();
    if (new_time > current_timepoint) {
        if (has_event_variables) {
            inertia_substitutions.clear();
            inertia_substitutions = current_timepoint_substitutions;
        }
        current_timepoint_substitutions.clear();
        current_timepoint = new_time;
    }
}

std::vector<std::shared_ptr<util::Grounding>>
RestrictedFilter::build_chase_facts(
    util::Timeline const &timeline, size_t previous_step,
    std::vector<std::shared_ptr<util::Grounding>> const &input_facts) {
    // auto database_facts = head_formula->get_conclusions_timepoint(timeline);
    // auto const &database_facts = database.get_data_full();
    auto current_time = timeline.get_time();
    for (auto const &input_fact : input_facts) {
        if (input_fact->is_fresh_sne(current_time, previous_step)) {
            // find_match(database_facts, input_fact);
            search_database(input_fact);
        }
    }
    return current_step_facts;
}

void RestrictedFilter::expire_outdated_groundings(
    util::Timeline const &timeline) {
    // head_formula->expire_outdated_groundings(timeline);
}

void RestrictedFilter::init_event_variable_list() {
    for (size_t i = 0; i < head_variables_count; i++) {
        auto const &var_name = head_variables.at(i);
        if (bound_variable_index.count(var_name) > 0) {
            auto bound_index = bound_variable_index.at(var_name);
            if (is_event_variable.at(bound_index)) {
                event_variable_list.push_back(i);
            }
        }
    }
}

void RestrictedFilter::init_body_head_var_index() {
    for (auto const &variable : head_variables) {
        int input_index = -1;
        if (free_variable_index.count(variable) > 0) {
            input_index = free_variable_index.at(variable);
        }
        body_head_var_index.push_back(input_index);
    }
}

void RestrictedFilter::init_head_atom_predicates(
    std::vector<std::unique_ptr<formula::Formula>> const &head_atoms) {
    for (auto const &head_atom : head_atoms) {
        auto const &predicate = head_atom->get_predicate_vector().at(0);
        head_atom_predicates.push_back(predicate);
    }
}

void RestrictedFilter::init_head_atoms_var_index_atom_to_head(
    std::vector<std::unique_ptr<formula::Formula>> const &head_atoms) {
    for (auto const &head_atom : head_atoms) {
        std::vector<int> var_index;
        // for (auto const &head_variable : head_variables) {
        // auto atom_variable = head_atom->get_variable_index(head_variable);
        // var_index.push_back(atom_variable);
        //}
        auto const &atom_variable_names = head_atom->get_variable_names();
        for (auto const &atom_variable : atom_variable_names) {
            auto head_index = head_variable_index.at(atom_variable);
            var_index.push_back(head_index);
        }
        head_atoms_var_index_atom_to_head.push_back(var_index);
    }
}

void RestrictedFilter::init_database_facts(util::Database &database) {
    database_facts.clear();
    for (auto const &predicate : head_atom_predicates) {
        database_facts.push_back(database.get_predicate_data(predicate));
    }
    has_database_facts = true;
}

std::string RestrictedFilter::generate_new_value(std::string const &var_name) {
    std::string result;
    if (use_global_nulls) {
        auto null_value_count =
            util::Global::get_instance().get_null_value_count();
        result = std::to_string(null_value_count) +
                 laser::util::special_value::CHASE_LABELED_NULL;
    } else {
        result = var_name + std::to_string(null_value_count);
        null_value_count++;
    }
    return result;
}

std::shared_ptr<util::Grounding> RestrictedFilter::generate_fact(
    std::vector<std::string> const &substitution,
    std::shared_ptr<util::Grounding> const &input_fact) const {
    auto result = input_fact->shallow_clone();
    result->set_constant_vector(substitution);
    return result;
}

std::vector<std::string> RestrictedFilter::extract_substitution(
    std::shared_ptr<util::Grounding> const &input_fact) const {
    std::vector<std::string> substitution;
    for (auto body_var_index : body_head_var_index) {
        if (body_var_index > 0) {
            auto const &value = input_fact->get_constant(body_var_index);
            substitution.push_back(value);
        } else {
            substitution.push_back(BOUND_VALUE_PLACEHOLDER);
        }
    }
    return substitution;
}

bool RestrictedFilter::is_substitution_match(
    std::vector<std::string> const &left,
    std::vector<std::string> const &right) const {
    for (size_t i = 0; i < left.size(); i++) {
        auto const &left_value = left.at(i);
        if (left_value != BOUND_VALUE_PLACEHOLDER) {
            auto const &right_value = right.at(i);
            if (left_value != right_value) {
                return false;
            }
        }
    }
    return true;
}

bool RestrictedFilter::has_match_current_timepoint(
    std::vector<std::string> const &initial_substitution) const {
    std::vector<std::vector<std::string>> result;
    // Look for frontier variable match in current_timepoint_substitutions
    for (auto const &substitution : current_step_substitutions) {
        if (is_substitution_match(initial_substitution, substitution)) {
            return true;
        }
    }
    return false;
}

bool RestrictedFilter::is_full_match(
    std::vector<std::string> const &substitution) const {
    for (auto const &value : substitution) {
        if (value == BOUND_VALUE_PLACEHOLDER) {
            return false;
        }
    }
    return true;
}

bool RestrictedFilter::is_fact_match(
    std::vector<std::string> const &substitution,
    std::shared_ptr<util::Grounding> const &fact,
    size_t head_atom_index) const {
    auto const &var_index_atom_to_head =
        head_atoms_var_index_atom_to_head.at(head_atom_index);
    auto atom_var_count = var_index_atom_to_head.size();
    for (size_t atom_var_index = 0; atom_var_index < atom_var_count;
         atom_var_index++) {
        auto head_var_index = var_index_atom_to_head.at(atom_var_index);
        if (head_var_index >= 0) {
            auto fact_value = fact->get_constant(atom_var_index);
            auto subst_value = substitution.at(head_var_index);
            if (subst_value != BOUND_VALUE_PLACEHOLDER &&
                fact_value != subst_value) {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::string>
RestrictedFilter::fact_extend(std::vector<std::string> const &substitution,
                              std::shared_ptr<util::Grounding> const &fact,
                              size_t head_atom_index) const {
    std::vector<std::string> result = substitution;
    auto const &var_index_atom_to_head =
        head_atoms_var_index_atom_to_head.at(head_atom_index);
    auto atom_var_count = var_index_atom_to_head.size();
    for (size_t atom_var_index = 0; atom_var_index < atom_var_count;
         atom_var_index++) {
        auto head_var_index = var_index_atom_to_head.at(atom_var_index);
        if (head_var_index >= 0) {
            auto fact_value = fact->get_constant(atom_var_index);
            auto subst_value = substitution.at(head_var_index);
            if (subst_value == BOUND_VALUE_PLACEHOLDER) {
                result.at(head_var_index) = fact_value;
            }
        }
    }
    return result;
}

std::vector<std::string>
RestrictedFilter::event_extend(std::vector<std::string> const &left,
                               std::vector<std::string> const &right) const {
    std::vector<std::string> result = left;
    for (auto index : event_variable_list) {
        result.at(index) = right.at(index);
    }
    return result;
}

std::vector<std::string> RestrictedFilter::generate_extension(
    std::vector<std::string> const &substitution) {
    std::vector<std::string> result = substitution;
    for (size_t i = 0; i < head_variables_count; i++) {
        auto value = substitution.at(i);
        if (value == BOUND_VALUE_PLACEHOLDER) {
            result.at(i) = generate_new_value(head_variables.at(i));
        }
    }
    return result;
}

void RestrictedFilter::search_database(
    std::shared_ptr<util::Grounding> const &input_fact) {
    auto initial_substitution = extract_substitution(input_fact);
    if (has_match_current_timepoint(initial_substitution)) {
        return;
    }
    // 1. If I have event variables, match event variables to subst from
    // previous events if frontier variables match
    std::vector<std::vector<std::string>> substitution_list;
    if (has_event_variables) {
        substitution_list = match_events(initial_substitution);
    }
    substitution_list.push_back(initial_substitution);
    // if all bound variables are events -> substitution is complete
    if (is_full_match(substitution_list.at(0))) {
        current_step_substitutions.push_back(substitution_list.at(0));
        auto new_fact = generate_fact(substitution_list.at(0), input_fact);
        current_step_facts.push_back(new_fact);
        return;
    }
    // 2. If substitution is not yet complete, iterate database
    for (auto const &substitution : substitution_list) {
        bool match_found = find_database_match(input_fact, substitution, 0);
        if (match_found) {
            return;
        }
    }
    // 3. If I reach this point, I did not found a full match.
    // substiturion_list may contain extensions of initial_substitution, with
    // event variables.
    auto extended_substitution = generate_extension(substitution_list.at(0));
    current_step_substitutions.push_back(extended_substitution);
    auto new_fact = generate_fact(substitution_list.at(0), input_fact);
    current_step_facts.push_back(new_fact);
}

std::vector<std::vector<std::string>> RestrictedFilter::match_events(
    std::vector<std::string> const &initial_substitution) const {
    std::vector<std::vector<std::string>> result;
    // Look for frontier and event match in inertia_substitutions
    for (auto const &substitution : inertia_substitutions) {
        if (is_substitution_match(initial_substitution, substitution)) {
            auto new_substitution =
                event_extend(initial_substitution, substitution);
            result.push_back(new_substitution);
        }
    }
    return result;
}

std::vector<std::vector<std::string>> RestrictedFilter::match_database(
    std::vector<std::string> const &initial_substitution,
    size_t head_atom_index) const {
    std::vector<std::vector<std::string>> result;
    // TODO for all facts in database_facts.at(head_atom_index)
    // if substition matches fact ->
    // create extended substitution -> add it to substitution list
    auto *database = database_facts.at(head_atom_index);
    for (auto const &fact : *database) {
        if (is_fact_match(initial_substitution, fact, head_atom_index)) {
            auto new_substitution =
                fact_extend(initial_substitution, fact, head_atom_index);
            result.push_back(new_substitution);
        }
    }
    return result;
}

bool RestrictedFilter::find_database_match(
    std::shared_ptr<util::Grounding> const &input_fact, 
    std::vector<std::string> const &initial_substitution,
    size_t head_atom_index) {
    auto substitution_list =
        match_database(initial_substitution, head_atom_index);
    if (substitution_list.empty()) {
        return false;
    }
    size_t next_head_atom = head_atom_index + 1;
    if (next_head_atom < head_atom_count) {
        for (auto const &substitution : substitution_list) {
            bool found_extension =
                find_database_match(input_fact, substitution, next_head_atom);
            if (found_extension) {
                return true;
            }
        }
    } else {
        // we have reached the last atom in the head -> we check if match
        // it is sufficient to check the first substitution
        auto substitution = substitution_list.at(0);
        if (is_full_match(substitution)) {
            current_step_substitutions.push_back(substitution);
            auto new_fact = generate_fact(substitution, input_fact);
            current_step_facts.push_back(new_fact);
            return true;
        }
    }
    return false;
}

// void RestrictedFilter::find_match(
// std::vector<std::shared_ptr<util::Grounding>> const &database,
// std::shared_ptr<util::Grounding> const &input_fact) {
// if (has_event_variables) {
// for (auto const &inertia_fact : current_timepoint_facts) {
// if (is_event_variable_match(inertia_fact, input_fact)) {
// auto new_fact = generate_chase_fact_from_inertia(inertia_fact);
// current_step_facts.push_back(new_fact);
// return;
//}
//}
// for (auto const &inertia_fact : inertia_facts) {
// if (is_event_variable_match(inertia_fact, input_fact)) {
// auto new_fact = generate_chase_fact_from_inertia(inertia_fact);
// current_step_facts.push_back(new_fact);
// return;
//}
//}
//}
// for (auto const &db_fact : database) {
// if (is_database_match(db_fact, input_fact)) {
// auto new_fact = convert_to_chase_fact(db_fact);
// current_step_facts.push_back(new_fact);
// return;
//}
//}
// auto chase_fact = generate_chase_fact(input_fact);
// current_step_facts.push_back(chase_fact);
//}

// bool RestrictedFilter::is_database_match(
// std::shared_ptr<util::Grounding> const &db_fact,
// std::shared_ptr<util::Grounding> const &input_fact) const {
//// return false if there is a match but the new grounding expires later
// return is_frontier_variable_match(db_fact, input_fact) &&
// input_fact->get_horizon_time() <= db_fact->get_horizon_time();
//}

// bool RestrictedFilter::is_event_variable_match(
// std::shared_ptr<util::Grounding> const &inertia_fact,
// std::shared_ptr<util::Grounding> const &input_fact) const {
//// TODO should I check all annotations or is ht enough?
// for (auto const &var_name : frontier_variables) {
// auto input_index = free_variable_index.at(var_name);
// auto const &input_value = input_fact->get_constant(input_index);
// auto const &event_value = inertia_fact->get_constant(input_index);
// if (event_value != input_value) {
// return false;
//}
//}
// return true;
//}

// bool RestrictedFilter::is_frontier_variable_match(
// std::shared_ptr<util::Grounding> const &db_fact,
// std::shared_ptr<util::Grounding> const &input_fact) const {
//// TODO should I check all annotations or is ht enough?
// for (auto const &var_name : frontier_variables) {
// auto input_index = free_variable_index.at(var_name);
// auto const &input_value = input_fact->get_constant(input_index);
// auto db_index = head_formula->get_variable_index(var_name);
// auto const &db_value = db_fact->get_constant(db_index);
// if (db_value != input_value) {
// return false;
//}
//}
// return true;
//}

// std::unique_ptr<formula::Formula> RestrictedFilter::build_head_formulas(
// std::vector<std::unique_ptr<formula::Formula>> const &list) const {
// bool merge = true;
// std::set<int> index_set;
// while(merge) {
// merge = false;
// for (int i = 0; i < list.size()-1; i++) {
// auto const &left = list.at(i);
// for (int j = i+1; j < list.size(); j++) {
// auto const &right = list.at(j);
// if (have_common_variables(i,j)){
// auto result = merge(left, right);
// merge = true;
// index_set.insert(i);
// index_set.insert(j);
//}
//}
//}

//}

//}

// std::unique_ptr<formula::Formula> RestrictedFilter::build_head_formula(
// size_t index,
// std::vector<std::unique_ptr<formula::Formula>> const &list) const {
// if (index == list.size() - 1) {
// auto result = list[index]->clone();
// result->set_head(false);
// return result;
//}
// auto left = list[index]->clone();
// left->set_head(false);
// auto right = build_head_formula(index + 1, list);
// return std::make_unique<formula::Conjunction>(std::move(left),
// std::move(right), true);
//}

// std::shared_ptr<util::Grounding> RestrictedFilter::convert_to_chase_fact(
// std::shared_ptr<util::Grounding> const &db_fact) {
// std::vector<std::string> chase_values;
// for (auto const &var_name : head_variables) {
// int head_index = head_formula->get_variable_index(var_name);
// if (head_index >= 0) {
// auto value = db_fact->get_constant(head_index);
// chase_values.push_back(value);
//} else {
// chase_values.push_back("filler_non-frontier_value");
//}
//}
// auto result = db_fact->shallow_clone();
// result->set_constant_vector(chase_values);
// return result;
//}

// std::shared_ptr<util::Grounding> RestrictedFilter::generate_chase_fact(
// std::shared_ptr<util::Grounding> const &input_fact) {
// std::vector<std::string> chase_values;
// std::vector<std::string> bound_values;
// for (auto const &var_name : bound_variables) {
// auto new_null = generate_new_value(var_name);
// bound_values.push_back(std::move(new_null));
//}
// for (auto const &var_name : head_variables) {
// if (bound_variable_index.count(var_name) > 0) {
// auto index = bound_variable_index.at(var_name);
// auto value = bound_values.at(index);
// chase_values.push_back(value);
//} else {
// auto index = free_variable_index.at(var_name);
// auto value = input_fact->get_constant(index);
// chase_values.push_back(value);
//}
//}
// auto result = input_fact->shallow_clone();
// result->set_constant_vector(chase_values);
// return result;
//}

// std::shared_ptr<util::Grounding>
// RestrictedFilter::generate_chase_fact_from_inertia(
// std::shared_ptr<util::Grounding> const &inertia_fact) {
// std::vector<std::string> chase_values;
// for (size_t i = 0; i < head_variables.size(); i++) {
// auto const &var_name = head_variables.at(i);
// std::string value;
// if ((bound_variable_index.count(var_name) > 0) &&
//(!is_event_variable.at(bound_variable_index.at(var_name)))) {
// value = generate_new_value(var_name);
//} else {
// value = inertia_fact->get_constant(i);
//}
// chase_values.push_back(value);
//}
// auto result = inertia_fact->shallow_clone();
// result->set_constant_vector(chase_values);
// if (result->get_horizon_time() < current_timepoint) {
// result->set_horizon_time(current_timepoint);
//}
// return result;
//}

} // namespace laser::rule
