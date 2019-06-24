//
// Created by mike on 6/19/18.
//

#ifndef LASER_UTIL_READ_EXCEPTION_H
#define LASER_UTIL_READ_EXCEPTION_H

#include <exception>

namespace laser {
namespace util {


class ReadException : public std::exception {
private:
    char const *message_m = "ReadException";
public:
    ReadException() = default;

    explicit ReadException(char const *message) : message_m(message) {}

    const char *what() const noexcept override {
        return message_m;
    }
};

} // namespace util
} // namespace laser


#endif // LASER_UTIL_READ_EXCEPTION_H
