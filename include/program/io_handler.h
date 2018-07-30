//
// Created by mike on 30.07.2018.
//

#ifndef LASER_PROGRAM_IO_HANDLER_H
#define LASER_PROGRAM_IO_HANDLER_H

#include <unordered_map>

#include "formula/formula.h"
#include "formula/extended/atom.h"
#include "io/io_manager.h"

namespace laser {
namespace program {

class IOHandler {

private:

io::IOManager *ioManager;

public:
    explicit IOHandler(io::IOManager *ioManager);

// methods
    std::unordered_map<std::string, std::vector<formula::Formula *>>
    get_stream_data(uint64_t time);

    void put_conclusions(
            uint64_t time,std::vector<formula::Formula *> conclusions);
};

} // namespace program
} // namespace laser


#endif // LASER_PROGRAM_IO_HANDLER_H
