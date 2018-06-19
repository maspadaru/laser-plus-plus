//
// Created by mike on 6/19/18.
//

#ifndef LASER_EXCEPTION_FORMAT_EXCEPTION_H
#define LASER_EXCEPTION_FORMAT_EXCEPTION_H

#include <exception>

namespace laser {
namespace exception {


class FormatException : public std::exception {
private:
    char *message_m = "FormatException";
public:
    FormatException() = default;
    FormatException(char *message) : message_m(message) {}

    const char *what() const noexcept {
        return message_m;
    }
};

} // namespace exception
} // namespace laser


#endif // LASER_EXCEPTION_FORMAT_EXCEPTION_H
