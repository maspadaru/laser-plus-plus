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
#include "util/data_atom.h"

namespace laser {
namespace program {

class IOHandler {

  private:
    uint64_t current_tuple_counter = 0;

    std::vector<std::shared_ptr<formula::Grounding>> remove_duplicates(
        std::vector<std::shared_ptr<formula::Grounding>> &input_groundings)
        const;

  public:
    // methods
    std::unordered_map<std::string,
                       std::vector<std::shared_ptr<formula::Grounding>>>
    handle_input(util::Timeline &timeline,
                 std::vector<util::DataAtom> const &data_vector);

    std::vector<util::DataAtom>
    handle_output(util::Timeline const &timeline,
                  std::vector<formula::Formula *> const &conclusions);
};

} // namespace program
} // namespace laser

#endif // LASER_PROGRAM_IO_HANDLER_H
