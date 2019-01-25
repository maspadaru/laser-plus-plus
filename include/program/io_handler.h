//
// Created by mike on 30.07.2018.
//

#ifndef LASER_PROGRAM_IO_HANDLER_H
#define LASER_PROGRAM_IO_HANDLER_H

#include <memory>
#include <unordered_map>

#include "formula/extended/atom.h"
#include "formula/grounding.h"
#include "io/io_manager.h"
#include "util/timeline.h"

namespace laser {
namespace program {

class IOHandler {

  private:
    io::IOManager *ioManager;
    // double reader_secs = 0;
    // double handler_secs = 0;
    uint64_t current_tuple_counter = 0;

    std::vector<std::shared_ptr<formula::Grounding>> remove_duplicates(
        std::vector<std::shared_ptr<formula::Grounding>> input_groundings)
        const;

  public:
    explicit IOHandler(io::IOManager *ioManager);

    // double get_reader_secs() const;
    // double get_handler_secs() const;

    // methods
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
    get_stream_data(laser::util::Timeline &timeline);

    void put_conclusions(util::Timeline timeline,
                         std::vector<formula::Formula *> const &conclusions);
};

} // namespace program
} // namespace laser

#endif // LASER_PROGRAM_IO_HANDLER_H
