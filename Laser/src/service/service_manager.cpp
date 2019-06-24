#include "service/service_manager.h"

namespace laser {
namespace service {

ServiceManager::ServiceManager() : in(std::cin), out(std::cout) {}

ServiceManager::ServiceManager(std::istream &input, std::ostream &output)
    : in(input), out(output) {}

bool ServiceManager::read_line(std::string &line) {
    std::getline(in, line);
    return in.good();
}

bool ServiceManager::read_request() {
    auto result = read_line(request_string);
    return result;
}

void ServiceManager::serve_requests() {
    out << " *** Timepoint: " << time << " *** " << std::endl;
    auto has_request = read_request();
    while (has_request) {
        RequestParser request_parser(database_facts);
        auto request = request_parser.parse_request(request_string);
        auto result = request->evaluate();
        out << "Query: " << request_string << " -> Answer: ";
        out << result;
        out << std::endl;
        has_request = read_request();
    }
}

void ServiceManager::update(
    uint64_t time, std::vector<std::shared_ptr<util::Grounding>> facts,
    std::vector<std::shared_ptr<util::Grounding>> conclusions) {
    this->time = time;
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
