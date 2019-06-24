#include "service/service_manager.h"

namespace laser {
namespace service {

bool ServiceManager::read_request() {
    request_string = "Q: A, C ? p(A, 1, C, 1)";
    return true;
}

void ServiceManager::serve_requests() {
    RequestParser request_parser(database_facts);
    bool has_request = true;
    while (has_request) {
        has_request = read_request();
        auto request = request_parser.parse_request(request_string);
        auto result = request->evaluate();
        std::cout << " Request Answer: "<< result << std::endl;
        // TODO
        has_request = false;
    }
}

void ServiceManager::update(
    std::vector<std::shared_ptr<util::Grounding>> facts,
    std::vector<std::shared_ptr<util::Grounding>> conclusions) {
    database_facts.clear();
    database_facts.insert(database_facts.end(),
                          std::make_move_iterator(facts.begin()),
                          std::make_move_iterator(facts.end()));
    database_facts.insert(database_facts.end(),
                          std::make_move_iterator(conclusions.begin()),
                          std::make_move_iterator(conclusions.end()));
}

} // namespace service
} // namespace laser
