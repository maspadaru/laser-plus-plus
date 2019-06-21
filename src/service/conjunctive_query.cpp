#include "service/conjunctive_query.h"

namespace laser {
namespace service {

std::string ConjunctiveQuery::evaluate() {
    return "Answer";

}

std::vector<std::shared_ptr<util::Grounding>> ConjunctiveQuery::build_result_vector() {
    std::vector<std::shared_ptr<util::Grounding>> result;
    // TODO: this shouls be all facts, no SNE applied
    //auto database_facts = query_formula->get_conclusions_timepoint(timeline);
    //auto current_time = timeline.get_time();
    //for (auto const &db_fact : database_facts) {
        //if (is_free_variable_match(db_fact, trigger)) {
            //result.push_back(db_fact);
        //}
    //}

    return result;

}

} // namespace service
} // namespace laser
