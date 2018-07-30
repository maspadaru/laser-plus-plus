//
// Created by mike on 7/17/18.
//

#ifndef LASER_IO_IO_MANAGER_H
#define LASER_IO_IO_MANAGER_H

#include "io/data_atom.h"

namespace laser {
namespace io {

class IOManager {
private:

// methods

public:

// constructors & destructors

    virtual ~IOManager() = default;

// getters & setters

// methods
    virtual uint64_t read_stream_start_time() = 0;

    virtual uint64_t read_stream_end_time() = 0;

    virtual std::vector<laser::io::DataAtom>
    read_stream_data(uint64_t time) = 0;

    virtual std::vector<laser::io::DataAtom>
    read_background_data() = 0;

    virtual void write_output_data(
            uint64_t time,
            std::vector<laser::io::DataAtom> output_vector) = 0;
};


} // namespace io
} // namespace laser

#endif // LASER_IO_IO_MANAGER_H
