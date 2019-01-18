//
// Created by mike on 6/15/18.
//

#include "formula/extended/atom.h"

namespace laser {
namespace formula {

// constructors & destructors

Atom::Atom(std::string predicate) { this->predicate = std::move(predicate); }

Atom::Atom(std::string predicate,
           std::vector<std::string> const &variable_names) {
    this->predicate = std::move(predicate);
    set_variable_names(variable_names);
}

void Atom::set_variable_names(std::vector<std::string> const &variable_names) {
    this->full_variable_names = variable_names;
    std::vector<std::string> result;
    std::unordered_map<std::string, int> temp;
    for (size_t index = 0; index < variable_names.size(); index++) {
        std::string name = variable_names.at(index);
        temp.try_emplace(name, -1);
        if (temp.at(name) >= 0) {
            binding_map.insert({index, temp.at(name)});
        } else {
            first_position_vector.push_back(index);
            result.push_back(name);
            temp.at(name) = index;
        }
    }
    grounding_table.set_variable_names(result);
}

Formula &Atom::create() const {
    auto result = new Atom();
    return *result;
}

Formula &Atom::clone() const {
    auto result = new Atom(*this);
    // TODO ? Is this enough?
    return *result;
}

Formula &Atom::move() {
    Atom *result = new Atom(std::move(this->predicate));
    result->is_head_m = this->is_head_m;
    result->type = this->type;
    result->full_variable_names = std::move(this->full_variable_names);
    result->binding_map = std::move(this->binding_map);
    result->first_position_vector = std::move(this->first_position_vector);
    result->grounding_table = std::move(this->grounding_table);
    return *result;
}

// getters & setters

FormulaType Atom::get_type() const { return this->type; }

std::vector<std::string> Atom::get_predicate_vector() const {
    std::vector<std::string> result;
    result.push_back(this->predicate);
    return result;
}

std::vector<std::string> Atom::get_variable_names() const {
    return grounding_table.get_variable_names();
}

std::vector<std::string> Atom::get_full_variable_names() const {
    return full_variable_names;
}

void Atom::set_head(bool is_head) { is_head_m = is_head; }

bool Atom::is_head() const { return is_head_m; }

// methods

size_t Atom::get_number_of_variables() const {
    return grounding_table.get_number_of_variables();
}

void Atom::expire_outdated_groundings(util::Timeline timeline) {
    grounding_table.expire_outdated_groundings(timeline.get_time(),
                                               timeline.get_tuple_count());
}

std::string Atom::debug_string() const {
    std::stringstream os;
    os << "ATOM -> predicate:  " << this->predicate;
    os << "; variable_names.size=: " << this->get_number_of_variables()
       << "; Names: ";
    for (auto const &variable : this->get_variable_names()) {
        os << variable << ", ";
    }
    os << "GroundingTable->size:" << grounding_table.get_size();
    return os.str();
}

std::vector<std::shared_ptr<Grounding>>
Atom::get_groundings(util::Timeline timeline) {
    return grounding_table.get_all_groundings();
}

std::vector<std::shared_ptr<Grounding>> Atom::get_conclusion_set() {
    std::set<std::shared_ptr<Grounding>, GroundingSubstitutionCompare>
        grounding_set;
    auto gt_groundings = grounding_table.get_all_groundings();
    for (auto const &grounding : gt_groundings) {
        grounding_set.insert(grounding);
    }
    std::vector<std::shared_ptr<Grounding>> result;
    result.insert(result.end(), grounding_set.begin(), grounding_set.end());
    return result;
}

std::vector<std::shared_ptr<Grounding>>
Atom::get_conclusions_timepoint(util::Timeline timeline) {
    // return grounding_table.get_all_groundings();
    return get_conclusion_set();
}

std::vector<std::shared_ptr<Grounding>>
Atom::get_conclusions_step(util::Timeline timeline) {
    return grounding_table.get_recent_groundings();
}

bool Atom::is_satisfied() const { return grounding_table.get_size() > 0; }

bool Atom::evaluate(
    util::Timeline timeline,
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<Grounding>>> const &facts) {
    if (facts.count(predicate) > 0) {
        auto grounding_vector = facts.at(predicate);
        for (auto const &grounding : grounding_vector) {
            accept(grounding);
        }
    }
    return grounding_table.has_recent_groundings();
}

void Atom::accept(std::shared_ptr<Grounding> const &grounding) {
    auto grounding_size = grounding->get_size();
    auto atom_size = this->get_number_of_variables();
    if (atom_size == grounding_size) {
        grounding_table.add_grounding(grounding);
    } else if (atom_size <= grounding_size && is_valid_fact(*grounding)) {
        auto valid_grounding = remove_duplicate_variables(*grounding);
        grounding_table.add_grounding(valid_grounding);
    } else {
        // TODO some sort of error
    }
}

int Atom::get_variable_index(std::string variable_name) const {
    return grounding_table.get_variable_index(variable_name);
}

bool Atom::is_valid_fact(Grounding const &grounding) const {
    bool is_valid = true;
    for (auto const &iterator : binding_map) {
        auto first = grounding.get_constant(iterator.first);
        auto second = grounding.get_constant(iterator.second);
        is_valid &= first == second;
    }
    return is_valid;
}

std::shared_ptr<Grounding>
Atom::remove_duplicate_variables(Grounding const &grounding) {
    std::vector<std::string> result_values;
    for (size_t index : first_position_vector) {
        result_values.push_back(grounding.get_constant(index));
    }
    Grounding result = Grounding(grounding.get_consideration_time(),
                                 grounding.get_horizon_time(),
                                 grounding.get_consideration_count(),
                                 grounding.get_horizon_count(), result_values);
    return std::make_shared<Grounding>(result);
}

void Atom::add_child(formula::Formula *child) {}

template <typename T>
void Atom::debug_print(std::string const &message, T const &value) const {
    std::cerr << "ATOM -> predicate: " << this->predicate << " -> ";
    std::cerr << message << " : " << value << std::endl;
}

} // namespace formula
} // namespace laser
