#ifndef LASER_SERVICE_REQUEST_H
#define LASER_SERVICE_REQUEST_H

#include <string>

namespace laser {
namespace service {

class Request {
  public:

    virtual ~Request() = default;

    virtual std::string evaluate() = 0;

};

} // namespace service 
} // namespace laser

#endif // LASER_SERVICE_REQUEST_H
