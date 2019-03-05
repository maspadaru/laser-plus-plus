//
// Created by mike on 02/25/19.
//

#include "reasoner/reasoner.h"

namespace laser {
namespace reasoner {

Reasoner::Reasoner(io::RuleReader *rule_reader, io::IOManager *io_manager)
    : rule_reader(rule_reader), io_manager(io_manager) {}

void Reasoner::insert_facts(uint64_t timepoint,
                            std::vector<util::DataAtom> &facts) {
    std::lock_guard<std::mutex> guard(fact_map_mutex);
    fact_map.try_emplace(timepoint, std::move(facts));
}

void Reasoner::remove_facts(uint64_t timepoint) {
    std::lock_guard<std::mutex> guard(fact_map_mutex);
    fact_map.erase(timepoint);
}

void Reasoner::insert_conclusions(uint64_t timepoint,
                                  std::vector<util::DataAtom> &conclusions) {
    std::lock_guard<std::mutex> guard(conclusion_map_mutex);
    conclusion_map.try_emplace(timepoint, std::move(conclusions));
}

void Reasoner::remove_conclusions(uint64_t timepoint) {
    std::lock_guard<std::mutex> guard(conclusion_map_mutex);
    conclusion_map.erase(timepoint);
}

void Reasoner::start() {
    util::Timeline main_timeline;
    auto start_time = io_manager->read_stream_start_time();
    auto end_time = io_manager->read_stream_end_time();
    main_timeline.set_start_time(start_time);
    main_timeline.set_min_time(start_time);
    main_timeline.set_max_time(end_time);
    std::thread read_thread(&Reasoner::read, this, main_timeline);
    std::thread evaluate_thread(&Reasoner::evaluate, this, main_timeline);
    std::thread write_thread(&Reasoner::write, this, main_timeline);
    read_thread.join();
    evaluate_thread.join();
    write_thread.join();
    std::cout << "Eval seconds: " << clock_eval.count() / 1000 << std::endl;
}

void Reasoner::read(util::Timeline timeline) {
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        auto facts = io_manager->read_stream_data(time);
        insert_facts(time, facts);
        timeline.increment_time();
        time = timeline.get_time();
    }
}

void Reasoner::evaluate(util::Timeline timeline) {
    auto program = program::Program(rule_reader);
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        bool has_new_input = fact_map.count(time) > 0;
        if (has_new_input) {
            auto clock_start = std::chrono::high_resolution_clock::now();
            auto const &facts = fact_map.at(time);
            auto conclusions = program.evaluate(timeline, facts);
            auto clock_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> clock_elapsed =
                clock_end - clock_start;
            clock_eval += clock_elapsed;
            insert_conclusions(time, conclusions);
            remove_facts(time);
            timeline.increment_time();
            time = timeline.get_time();
        } else {
            std::this_thread::yield();
        }
    }
}

void Reasoner::write(util::Timeline timeline) {
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        bool has_new_output = conclusion_map.count(time) > 0;
        if (has_new_output) {
            auto &conclusions = conclusion_map.at(time);
            io_manager->write_output_data(time, conclusions);
            remove_conclusions(time);
            timeline.increment_time();
            time = timeline.get_time();
        } else {
            std::this_thread::yield();
        }
    }
}

} // namespace reasoner
} // namespace laser
