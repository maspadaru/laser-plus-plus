//
// Created by mike on 6/17/18.
//

#include "input/input_manager.h"

namespace laser {
namespace input {

// constructors & destructors

// getters & setters

unsigned long long int InputManager::get_stream_start_time() const {
    return stream_start_time;
}

unsigned long long int InputManager::get_stream_end_time() const {
    return stream_end_time;
}

// methods

void InputManager::initialize_rule_reader(RuleReader *rule_reader,
                                          RuleParser *rule_parser) {
    this->rule_reader = rule_reader;
    this->rule_parser = rule_parser;
    this->is_initialised_rule_reader_m = true;
}

void
InputManager::initialize_background_reader(DataReader *background_data_reader,
                                           DataParser *background_data_parser) {
    this->background_data_reader = background_data_reader;
    this->background_data_parser = background_data_parser;
    this->is_initialised_background_reader_m = true;
}




void InputManager::initialize_stream_reader(DataReader *stream_data_reader,
                                            DataParser *stream_data_parser) {
    this->stream_data_reader = stream_data_reader;
    this->stream_data_parser = stream_data_parser;
    this->is_initialised_stream_reader_m = true;
}

bool InputManager::fetch_stream_metadata() {
    if (!is_initialised_stream_reader_m) {
        throw exception::UninitializedException("The background data reader "
                                                "was not initialised. Call "
                                                "initialize_stream_reader() "
                                                "before calling "
                                                "get_stream_facts()");
    }
    if (!has_metadata) {
        if (this->stream_data_reader->fetch_metadata()) {
            this->stream_start_time =
                    this->stream_data_reader->get_stream_start_time();
            this->stream_end_time =
                    this->stream_data_reader->get_stream_end_time();
            this->has_metadata = true;
        }
    }
    return has_metadata;
}

std::vector<rule::Rule *> InputManager::get_rules() const {
    if (!is_initialised_rule_reader_m) {
        throw exception::UninitializedException("The rule reader was not "
                                                "initialised. Call "
                                                "initialize_rule_reader() "
                                                "before calling get_rules()");
    }
    auto raw_rules = rule_reader->read_rules();
    auto parsed_rules = rule_parser->parse_rules(raw_rules);
    return parsed_rules;
}

std::tuple<int,
        std::unordered_map<std::string, std::vector<formula::Formula *>>>
InputManager::get_background_facts() const {
    if (!is_initialised_background_reader_m) {
        throw exception::UninitializedException("The background data reader "
                                                "was not initialised. "
                                                "Call initialize_background_reader() "
                                                "before calling get_background_facts()");
    }
    auto raw_background_facts = background_data_reader->read_all_data();
    int number_of_parsed_facts;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            parsed_background_facts_map;
    std::tie(number_of_parsed_facts, parsed_background_facts_map)
            = background_data_parser->parse_data(raw_background_facts);
    return std::make_tuple(number_of_parsed_facts,
                           parsed_background_facts_map);
}

std::tuple<unsigned long long int, unsigned long long int,
        std::unordered_map<std::string, std::vector<formula::Formula *>>>
InputManager::get_stream_facts(unsigned long long int request_time_point) {
    if (!is_initialised_stream_reader_m) {
        throw exception::UninitializedException("The background data reader "
                                                "was not initialised. Call "
                                                "initialize_stream_reader() "
                                                "before calling "
                                                "get_stream_facts()");
    }
    if (request_time_point != stream_current_time + 1) {
        throw exception::RequestException("the requested time point is"
                                          "not equal to the next time point of "
                                          "the stream");
    }

    int time_point_of_facts;
    std::vector<std::string> raw_stream_facts;
    std::tie(time_point_of_facts, raw_stream_facts)
            = stream_data_reader->read_next_data(request_time_point);
    stream_current_time++;
    if (time_point_of_facts != stream_current_time) {
        throw exception::RequestException("the requested time point could"
                                          "not be read from the stream ");
    }

    int number_of_facts;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            parsed_stream_facts_map;
    std::tie(number_of_facts, parsed_stream_facts_map)
            = stream_data_parser->parse_data(raw_stream_facts);
    stream_tuple_counter += number_of_facts;
    return std::make_tuple(this->stream_current_time,
                           this->stream_tuple_counter,
                           parsed_stream_facts_map);

}

bool InputManager::is_initialised_rule_reader() const {
    return is_initialised_rule_reader_m;
}

bool InputManager::is_initialised_background_reader() const {
    return is_initialised_background_reader_m;
}

bool InputManager::is_initialised_stream_reader() const {
    return is_initialised_stream_reader_m;
}


} // namespace input
} // namespace laser
