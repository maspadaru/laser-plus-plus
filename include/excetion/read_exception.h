//
// Created by mike on 6/19/18.
//

#ifndef LASER_EXCEPTION_READ_EXCEPTION_H
#define LASER_EXCEPTION_READ_EXCEPTION_H

#include <exception>

namespace laser {
namespace exception {


class ReadException : public std::exception {
private:
    char *message_m = "ReadException";
public:
    ReadException() = default;
    ReadException(char *message) : message_m(message) {}

    const char *what() const noexcept {
        return message_m;
    }
};

} // namespace exception
} // namespace laser


#endif // LASER_EXCEPTION_READ_EXCEPTION_H
