#ifndef LASER_SERVICE_SERVICE_MANAGER_H
#define LASER_SERVICE_SERVICE_MANAGER_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>

#include "service/query.h"
#include "service/request.h"
#include "service/request_parser.h"
#include "service/request_type.h"

namespace laser {
namespace service {

class ServiceManager {
  private:
    std::vector<std::shared_ptr<util::Grounding>> database_facts;
    std::string request_string;
    std::istream &in;
    std::ostream &out;

    bool read_request();

    bool read_line(std::string &line);

  public:
    explicit ServiceManager();
    explicit ServiceManager(std::istream &input, std::ostream &output);
    virtual ~ServiceManager() = default;

    void update(std::vector<std::shared_ptr<util::Grounding>> facts,
                std::vector<std::shared_ptr<util::Grounding>> conclusions);

    void serve_requests();
};

} // namespace service
} // namespace laser

#endif // LASER_SERVICE_SERVICE_MANAGER_H
