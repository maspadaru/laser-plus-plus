#include "service/query.h"

namespace laser {
namespace service {

std::string Query::evaluate() { 
    auto result_vector = build_result_vector();
    std::stringstream os;
    for (auto const &fact : result_vector) {
        os << fact->to_atom_string() << ", ";
    }
    return os.str();
}

Query::Query(formula::Formula *q_formula, util::Grounding trigger,
             std::vector<size_t> known_var_index,
             std::vector<std::shared_ptr<util::Grounding>> const &db_facts)
    : query_formula(&q_formula->move()), trigger(std::move(trigger)),
      known_index_vector(std::move(known_var_index)), database_facts(db_facts) {
    this->query_predicate = this->query_formula->get_predicate_vector().at(0);
}

Query::Query(Query const &other) : database_facts(other.database_facts) {
    this->query_formula = &query_formula->clone();
    this->trigger = other.trigger;
    this->query_predicate = other.query_predicate;
    this->known_index_vector = other.known_index_vector;
    this->request_time = other.request_time;
}

Query::Query(Query &&other) noexcept : database_facts(other.database_facts) {
    this->query_formula = &query_formula->move();
    this->trigger = std::move(other.trigger);
    this->query_predicate = std::move(other.query_predicate);
    this->known_index_vector = std::move(other.known_index_vector);
    this->request_time = other.request_time;
}

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
        for (auto index : known_index_vector) {
            auto const &trigger_value = trigger.get_constant(index);
            auto const &db_value = db_fact->get_constant(index);
            if (db_value != trigger_value) {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<util::Grounding>> Query::build_result_vector() {
    std::vector<std::shared_ptr<util::Grounding>> result;
    for (auto const &db_fact : database_facts) {
        if (is_known_variable_match(db_fact)) {
            result.push_back(db_fact);
        }
    }
    return result;
}

} // namespace service
} // namespace laser
