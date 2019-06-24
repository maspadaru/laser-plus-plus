#ifndef LASER_SERVICE_CONJUNCTIVE_QUERY_H
#define LASER_SERVICE_CONJUNCTIVE_QUERY_H

#include <string>

#include "formula/formula.h"
#include "service/request.h"
#include "service/request_type.h"
#include "util/grounding.h"

namespace laser {
namespace service {

class ConjunctiveQuery : public Request {
  private:
    RequestType type = RequestType::CONJUNCTIVE_QUERY;
    std::unique_ptr<formula::Formula> query_formula;
    util::Grounding trigger;
    util::Timeline timeline;
    std::vector<std::string> free_variables();
    std::vector<std::string> bound_variables();

    void init(formula::Formula *query_formula,
                util::Grounding trigger,
                util::Timeline const &timeline,
                std::vector<std::string> const &query_variables,
                std::vector<std::string> const &free_variables,
                std::vector<std::string> const &bound_variables,
                util::Database const &database);


    std::vector<std::shared_ptr<util::Grounding>> build_result_vector();

  public:
    ~ConjunctiveQuery() override = default;

    std::string evaluate() override;
};

} // namespace service
} // namespace laser

#endif // LASER_SERVICE_CONJUNCTIVE_QUERY_H
