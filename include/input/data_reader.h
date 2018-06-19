//
// Created by mike on 6/17/18.
//

#ifndef LASER_INPUT_DATA_READER_H
#define LASER_INPUT_DATA_READER_H


#include <string>
#include <vector>
#include <unordered_map>

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
     * Reads data from input up to the next time point. Useful for reading
     * stream data.
     * @return Unordered Map where the key is the time point of the data and
     * the value is a vector of strings containing raw data read from the input.
     * @throw ReadException if any problem occurred while reading the input,
     * e.g.: source file is inaccessible
     */
    virtual std::unordered_map<long long int, std::vector<std::string>>
    read_next_data() = 0;

};


} // namespace input
} // namespace laser


#endif // LASER_INPUT_DATA_READER_H
