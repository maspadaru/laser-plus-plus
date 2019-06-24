#ifndef LASER_SERVICE_QUERY_H
#define LASER_SERVICE_QUERY_H

#include <string>

#include "formula/formula.h"
#include "rule/shared.h"
#include "service/request.h"
#include "service/request_type.h"
#include "util/grounding.h"

namespace laser {
namespace service {

class Query : public Request {
  private:
    RequestType type = RequestType::QUERY;
    formula::Formula *query_formula;
    std::string query_predicate;
    util::Grounding trigger;
    std::vector<size_t> known_index_vector;
    std::vector<std::shared_ptr<util::Grounding>> const &database_facts;
    uint64_t request_time;

    //void
    //init(formula::Formula *query_formula, util::Grounding trigger,
         //std::vector<std::string> known_variables,
         //std::vector<std::shared_ptr<util::Grounding>> const &database_facts);

    bool is_known_variable_match(
        std::shared_ptr<util::Grounding> const &db_fact) const;

    std::vector<std::shared_ptr<util::Grounding>> build_result_vector();

  public:
    explicit Query(
        formula::Formula *query_formula, util::Grounding trigger,
        std::vector<size_t> known_index_vector,
        std::vector<std::shared_ptr<util::Grounding>> const &database_facts);

    Query(Query const &other);
    Query(Query &&other) noexcept;

    ~Query() override = default;

    std::string evaluate() override;
};

} // namespace service
} // namespace laser

#endif // LASER_SERVICE_QUERY_H
