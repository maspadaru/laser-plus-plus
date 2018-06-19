//
// Created by mike on 6/17/18.
//

#include "input/input_manager.h"

namespace laser {
namespace input {

// constructors & destructors

// getters & setters

long long int InputManager::get_stream_start_time() const {
    return 0;
}

long long int InputManager::get_stream_end_time() const {
    return 0;
}

// methods

void InputManager::initialize_rule_reader(RuleReader *rule_reader,
                                          RuleParser *rule_parser) {

}

void
InputManager::initialize_background_reader(DataReader *background_data_reader,
                                           DataParser *background_data_parser) {

}

void InputManager::initialize_stream_reader(DataReader *stream_data_reader,
                                            DataParser *stream_data_parser) {

}

std::vector<rule::Rule *> InputManager::get_rules() const {
    return std::vector<rule::Rule *>();
}

std::unordered_map<std::string, std::vector<formula::Formula *>>
InputManager::get_background_facts() const {
    return std::unordered_map<std::string, std::vector<formula::Formula *>>();
}

std::tuple<long long int, long long int,
        std::unordered_map<std::string, formula::Formula *>>
InputManager::get_stream_facts_up_to(long long int time_point) {
    return std::tuple<long long int,
            long long int,
            std::unordered_map<std::string, formula::Formula *>>();
}


} // namespace input
} // namespace laser
