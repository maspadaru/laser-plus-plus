//
// Created by mike on 6/19/18.
//

#ifndef LASER_EXCEPTION_WRITE_EXCEPTION_H
#define LASER_EXCEPTION_WRITE_EXCEPTION_H

#include <exception>

namespace laser {
namespace exception {


class WriteException : public std::exception {
private:
    char *message_m = "WriteException";
public:
    WriteException(char *message) : message_m(message) {}

    const char *what() const {
        return message_m;
    }
};

} // namespace exception
} // namespace laser


#endif // LASER_EXCEPTION_WRITE_EXCEPTION_H
