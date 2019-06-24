//
// Created by mike on 6/19/18.
//

#ifndef LASER_UTIL_UNINITIALIZED_EXCEPTION_H
#define LASER_UTIL_UNINITIALIZED_EXCEPTION_H

#include <exception>

namespace laser {
namespace util {


class UninitializedException : public std::exception {
private:
    char const *message_m = "UninitializedException";
public:
    UninitializedException() = default;

    explicit UninitializedException(char const *message) : message_m(message) {}

    const char *what() const noexcept override {
        return message_m;
    }
};

} // namespace util
} // namespace laser


#endif // LASER_UTIL_UNINITIALIZED_EXCEPTION_H
