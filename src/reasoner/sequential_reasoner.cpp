//
// Created by mike on 02/25/19.
//

#include "reasoner/sequential_reasoner.h"

namespace laser {
namespace reasoner {

void SequentialReasoner::start() {
    Timeline main_timeline;
    main_timeline.set_min_time(ioManager->read_stream_start_time());
    main_timeline.set_max_time(ioManager->read_stream_end_time());

    // create program

    read(main_timeline);
    evaluate(main_timeline);
    write(main_timeline);
}

void SequentialReasoner::read(Timeline timeline) {
    IOHandler
    while (!timeline.is_past_max_time()) {
        auto facts = ioHandler.get_stream_data(timeline);
        fact_map.try_emplace(timeline.get_time(), facts);
        timeline.increment_time();
    }
}

void SequentialReasoner::evaluate(Timeline timeline) {
    program = new Program();
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        if (fact_map.count(time) > 0) {
            auto facts = fact_map.at(time);
            auto conclusions = program.evaluate(timeline, facts);
            conclusion_map.try_emplace(time, conclusions);
            fact_map.erase(time);
            timeline.increment_time();
            time = timeline.get_time();
        }
    }
}

void SequentialReasoner::write(Timeline timeline) {
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        if (conclusion_map.count(time) > 0) {
            auto conclusions = conclusion_map.at(time);
            ioHandler.put_conclusions(timeline, conclusions);
            conclusion_map.erase(time);
            timeline.increment_time();
            time = timeline.get_time();
        }
    }
}

} // namespace reasoner
} // namespace laser
