#ifndef LASER_SERVICE_SERVICE_MANAGER_H
#define LASER_SERVICE_SERVICE_MANAGER_H

#include <iostream>
#include <memory>
#include <vector>

#include "service/query.h"
#include "service/request.h"
#include "service/request_parser.h"
#include "service/request_type.h"

namespace laser {
namespace service {

class ServiceManager {
  private:
    std::string request_string;

    bool read_request();
    std::vector<std::shared_ptr<util::Grounding>> database_facts;

  public:
    virtual ~ServiceManager() = default;

    void update(std::vector<std::shared_ptr<util::Grounding>> facts,
                std::vector<std::shared_ptr<util::Grounding>> conclusions);

    void serve_requests();
};

} // namespace service
} // namespace laser

#endif // LASER_SERVICE_SERVICE_MANAGER_H
