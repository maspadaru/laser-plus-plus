//
// Created by mike on 02/25/19.
//

#include "reasoner/sequential_reasoner.h"

namespace laser {
namespace reasoner {

SequentialReasoner::SequentialReasoner(io::RuleReader *rule_reader,
                                       io::IOManager *io_manager)
    : rule_reader(rule_reader), io_manager(io_manager) {}

void SequentialReasoner::start() {
    util::Timeline main_timeline;
    auto start_time = io_manager->read_stream_start_time();
    auto end_time = io_manager->read_stream_end_time();
    main_timeline.set_start_time(start_time);
    main_timeline.set_min_time(start_time);
    main_timeline.set_max_time(end_time);
    std::thread read_thread(&SequentialReasoner::read, this, main_timeline);
    std::thread evaluate_thread(&SequentialReasoner::evaluate, this, main_timeline);
    std::thread write_thread(&SequentialReasoner::write, this, main_timeline);
    read_thread.join();
    evaluate_thread.join();
    write_thread.join();
}

void SequentialReasoner::read(util::Timeline timeline) {
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        auto facts = io_manager->read_stream_data(time);
        fact_map.try_emplace(timeline.get_time(), facts);
        timeline.increment_time();
        time = timeline.get_time();
    }
}

void SequentialReasoner::evaluate(util::Timeline timeline) {
    auto program = program::Program(rule_reader); 
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        bool has_new_input = fact_map.count(time) > 0;
        if (has_new_input) {
            auto facts = fact_map.at(time);
            auto conclusions = program.evaluate(timeline, facts);
            conclusion_map.try_emplace(time, conclusions);
            fact_map.erase(time);
            timeline.increment_time();
            time = timeline.get_time();
        }
    }
}

void SequentialReasoner::write(util::Timeline timeline) {
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        bool has_new_output = conclusion_map.count(time) > 0;
        if (has_new_output) {
            auto conclusions = conclusion_map.at(time);
            io_manager->write_output_data(time, conclusions);
            conclusion_map.erase(time);
            timeline.increment_time();
            time = timeline.get_time();
        }
    }
}

} // namespace reasoner
} // namespace laser
