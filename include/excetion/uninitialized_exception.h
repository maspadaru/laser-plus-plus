//
// Created by mike on 6/19/18.
//

#ifndef LASER_EXCEPTION_UNINITIALIZED_EXCEPTION_H
#define LASER_EXCEPTION_UNINITIALIZED_EXCEPTION_H

#include <exception>

namespace laser {
namespace exception {


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

} // namespace exception
} // namespace laser


#endif // LASER_EXCEPTION_UNINITIALIZED_EXCEPTION_H
