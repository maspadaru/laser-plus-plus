//
// Created by mike on 6/17/18.
//

#ifndef LASER_INPUT_DATA_READER_H
#define LASER_INPUT_DATA_READER_H


#include <string>
#include <vector>
#include <tuple>

namespace laser {
namespace input {

/**
 *  DataReader Interface
 */
class DataReader {
private:
public:
// constructors & destructors
    virtual ~DataReader() = default;
// getters & setters

    virtual bool has_metadata() const = 0;

    virtual long long int get_stream_start_time() const = 0;

    virtual long long int get_stream_end_time() const = 0;

// const methods

    /**
     * Reads all data from input. Useful for reading background data
     * @return Vector of strings containing raw data read from the input.
     * @throw ReadException if any problem occurred while reading the input,
     * e.g.: source file is inaccessible
     */
    virtual std::vector<std::string> read_all_data() const = 0;

// methods

    /**
     * Reads facts from input coresponding to the requested time point. Useful
     * for reading stream data.
     * @param request_time_point
     * @return
     *      Tuple: [1] The stream time point of the facts that were read
     *             [2] A vector of strings containing raw data read from the
     *                 input.
     * @throw ReadException if any problem occurred while reading the input,
     * e.g.: source file is inaccessible
     */
    virtual std::tuple<long long int, std::vector<std::string>>
    read_next_data(long long int request_time_point) = 0;

    /**
     * Attempts to fetch metadata information about the data source, such the
     * timeline of the stream. This metadata is stored as local state.
     *
     * @return True if retrieval of metadata is successful.
     * @throw ReadException if any problem occurred while reading the input,
     * e.g.: source file is inaccessible
     */
    virtual bool fetch_metadata() = 0;

};


} // namespace input
} // namespace laser


#endif // LASER_INPUT_DATA_READER_H
