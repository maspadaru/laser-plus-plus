//
// Created by mike on 6/17/18.
//

#ifndef LASER_INPUT_INPUT_MANAGER_H
#define LASER_INPUT_INPUT_MANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <tuple>

#include "formula/formula.h"
#include "rule/rule.h"
#include "input/data_reader.h"
#include "input/data_parser.h"
#include "input/rule_reader.h"
#include "input/rule_parser.h"
#include "excetion/request_exception.h"
#include "excetion/read_exception.h"
#include "excetion/uninitialized_exception.h"

namespace laser {
namespace input {

class InputManager {
private:
    uint64_t stream_current_time = 0;
    uint64_t stream_tuple_counter = 0;
    uint64_t stream_start_time = 0;
    uint64_t stream_end_time = 0;
    bool has_metadata = false;

    RuleReader *rule_reader = nullptr;
    RuleParser *rule_parser = nullptr;
    DataReader *background_data_reader = nullptr;
    DataParser *background_data_parser = nullptr;
    DataReader *stream_data_reader = nullptr;
    DataParser *stream_data_parser = nullptr;

    bool is_initialised_rule_reader_m = false;
    bool is_initialised_background_reader_m = false;
    bool is_initialised_stream_reader_m = false;


public:
// constructors & destructors

    InputManager() = default;

    ~InputManager() = default;

// getters & setters

    uint64_t get_stream_start_time() const;

    uint64_t get_stream_end_time() const;

    bool is_initialised_rule_reader() const;

    bool is_initialised_background_reader() const;

    bool is_initialised_stream_reader() const;

//methods

    /**
     * Reads all the rules read by the rule reader.
     * @return Vector of pointers to rule objects.
     * @throw ReadException if any problem occurred while reading the input,
     *      e.g.: source file is inaccessible
     * @throw FormatException if the input is not in a format that can be
     *      properly parsered.
     * @throw UninitializedException if the rule reader was not
     *      initialised by calling initialize_rule_reader() before calling
     *      get_rules().
     */
    std::vector<rule::Rule> get_rules();

    /**
     * Reads all the fact from the background datastore.
     * A parser that reads background_data should also annotate it as such, i.e.
     * grounding.is_background_fact := true
     * @return
     *      Tuple: [1] The number background facts read.
     *             [2] Unordered Map where the keys are predicates and the
     *             values are vectors containing all facts that have the
     *             corresponding key as predicate.
     * @throw ReadException if any problem occurred while reading the input,
     *      e.g.: source file is inaccessible
     * @throw FormatException if the input is not in a format that can be
     *      properly parsered.
     * @throw UninitializedException if  the background data reader was not
     *      initialised by calling initialize_background_reader() before calling
     *      get_background_facts().
     */
    std::tuple<int, std::unordered_map<std::string, std::vector<formula::Formula *>>>
    get_background_facts();

    /**
     * Reads the facts from the stream that occurred at the requested time point
     * Updates curent time and tuple counters.
     * @param request_time_point
     * @return
     *      Tuple: [1] The stream time point corresponding to these facts
     *             [2] The stream tuple counter of the last fact
     *             [3] Unordered Map where the keys are predicates and the
     *             values are vectors containing facts that have the
     *             corresponding key as predicate.
     * @throw ReadException if any problem occurred while reading the input,
     *      e.g.: source file is inaccessible
     * @throw FormatException if the input is not in a format that can be
     *      properly parsered.
     * @throw RequestException if requested time point is
     *      not equal to the next time point of the stream, i. e. can
     *      not read out-of-order or facts from the past.
     * @throw UninitializedException if the stream data reader was not
     *      initialised by calling initialize_stream_reader() before calling
     *      get_stream_facts().
     */
    std::tuple<uint64_t, uint64_t, std::unordered_map<std::string,
            std::vector<formula::Formula *>>>
    get_stream_facts(uint64_t request_time_point);


    /**
     * return true on successful fetch
     * @throw ReadException if any problem occurred while reading the input,
     *      e.g.: source file is inaccessible
     * @throw UninitializedException if the stream data reader was not
     *      initialised by calling initialize_stream_reader() before calling
     *      fetch_stream_metadata().
     */
    bool fetch_stream_metadata();


    void initialize_rule_reader(
            RuleReader *rule_reader,
            RuleParser *rule_parser);

    void initialize_background_reader(
            DataReader *background_data_reader,
            DataParser *background_data_parser);

    void initialize_stream_reader(
            DataReader *stream_data_reader,
            DataParser *stream_data_parser);


};

} // namespace input
} // namespace laser


#endif // LASER_INPUT_INPUT_MANAGER_H
