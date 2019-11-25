#include "service/query.h"

namespace laser {
namespace service {

std::string Query::evaluate() {
    auto result_set = compute_answer();
    std::stringstream os;
    for (auto const &fact : result_set) {
        os << fact->to_atom_string() << "; ";
    }
    return os.str();
}

Query::Query(std::unique_ptr<formula::Formula> q_formula,
             util::Grounding trigger, std::vector<size_t> known_var_index,
             std::vector<std::shared_ptr<util::Grounding>> const &db_facts)
    : query_formula(std::move(q_formula)), trigger(std::move(trigger)),
      known_index_vector(std::move(known_var_index)), database_facts(db_facts) {
    this->query_predicate = this->query_formula->get_predicate_vector().at(0);
    formula_arity = this->query_formula->get_number_of_variables();
    request_time = 0;
}

// Query::Query(Query const &other) : database_facts(other.database_facts) {
// this->query_formula = &query_formula->clone();
// this->trigger = other.trigger;
// this->query_predicate = other.query_predicate;
// this->known_index_vector = other.known_index_vector;
// this->request_time = other.request_time;
//}

// Query::Query(Query &&other) noexcept : database_facts(other.database_facts) {
// this->query_formula = &query_formula->move();
// this->trigger = std::move(other.trigger);
// this->query_predicate = std::move(other.query_predicate);
// this->known_index_vector = std::move(other.known_index_vector);
// this->request_time = other.request_time;
//}

// void Query::init(
// formula::Formula *query_formula, util::Grounding trigger,
// std::vector<std::string> known_variables,
// std::vector<std::shared_ptr<util::Grounding>> const &database_facts) {
// this->query_formula = std::make_unique<formula::Formula>(query_formula);
// this->trigger = std::move(trigger);
// this->query_predicate = this->query_formula->get_predicate_vector().at(0);
// this->known_variables = std::move(known_variables);
// this->database_facts = database_facts;
//}

bool Query::is_known_variable_match(
    std::shared_ptr<util::Grounding> const &db_fact) const {
    if (query_predicate == db_fact->get_predicate()) {
        auto db_fact_arity = db_fact->get_size();
        if (formula_arity == db_fact_arity) {
            for (auto index : known_index_vector) {
                auto const &trigger_value = trigger.get_constant(index);
                auto const &db_value = db_fact->get_constant(index);
                if (db_value != trigger_value) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

std::set<std::shared_ptr<util::Grounding>,
         util::GroundingPredicateSubstitutionCompare>
Query::compute_answer() {
    std::set<std::shared_ptr<util::Grounding>,
             util::GroundingPredicateSubstitutionCompare>
        result;
    for (auto const &db_fact : database_facts) {
        if (is_known_variable_match(db_fact)) {
            result.insert(db_fact);
        }
    }
    return result;
}

} // namespace service
} // namespace laser
