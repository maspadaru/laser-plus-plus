//
// Created by mike on 6/15/18.
//

#include "program/program.h"


namespace laser {
namespace program {

// constructors & destructors

Program::~Program() {
    rule_vector.clear();
}

Program::Program(input::InputManager input_manager,
                 output::OutputManager output_manager) : input_manager(
        input_manager), output_manager(output_manager) {

    rule_vector = input_manager.get_rules();
    strata.stratify(rule_vector);

    if (input_manager.is_initialised_background_reader()) {
        std::unordered_map<std::string, std::vector<formula::Formula *>>
                background_facts;
        std::tie(number_of_background_facts, background_facts)
                = input_manager.get_background_facts();
        if (number_of_background_facts > 0) {
            strata.evaluate(current_time, current_tuple_counter,
                            background_facts);
        }
    }

    if (input_manager.fetch_stream_metadata()) {
        stream_start_time = input_manager.get_stream_start_time();
        stream_end_time = input_manager.get_stream_end_time();
    }
}

// getters & setters

// methods

void Program::expire_outdated_groundings(long long int current_time,
                                         long long int current_tuple_counter) {
    for (auto rule :rule_vector) {
        rule->expire_outdated_groundings(current_time, current_tuple_counter);
    }
}

bool Program::eval(long long int request_time_point) {
    bool derived_new_conclusions = false;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            stream_facts;
    std::tie(current_time, current_tuple_counter, stream_facts)
            = input_manager.get_stream_facts(request_time_point);
    expire_outdated_groundings(current_time, current_tuple_counter);
    derived_new_conclusions = strata.evaluate(current_time,
                                              current_tuple_counter,
                                              stream_facts);
    return derived_new_conclusions;
}

std::unordered_map<std::string, std::vector<formula::Formula *>>
Program::get_new_conclusions() {
    // TODO Get new conclusions from each rule as facts of type Formula*
    // TODO combine them in a map
    return std::unordered_map<std::string, std::vector<formula::Formula *>>();
}

void Program::evaluate(long long int request_time_point) {

    if (request_time_point != current_time + 1) {
        throw exception::RequestException("the requested time point is"
                                          "not equal to the next time point of "
                                          "the stream");
    }
    bool derived_new_conclusions = eval(request_time_point);
    if (derived_new_conclusions) {
        auto new_conclusions =  get_new_conclusions();
    }
    // TODO use output_manager to write the output
}

} // namespace program
} // namespace laser
