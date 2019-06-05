//
// Created by mike on 02/25/19.
//

#include "core/reasoner.h"

namespace laser {
namespace core {

Reasoner::Reasoner(rule::RuleReader *rule_reader, IOManager *io_manager,
                   util::ChaseAlgorithm chase_algorithm)
    : rule_reader(rule_reader), io_manager(io_manager), clock_eval(0) {
    laser::util::Settings::get_instance().set_chase_algorithm(chase_algorithm);
}

void Reasoner::insert_facts(
    uint64_t timepoint, std::vector<std::shared_ptr<util::Grounding>> facts) {
    std::lock_guard<std::mutex> guard(fact_map_mutex);
    fact_map.try_emplace(timepoint, std::move(facts));
}

std::vector<std::shared_ptr<util::Grounding>>
Reasoner::get_facts(uint64_t timepoint) {
    std::lock_guard<std::mutex> guard(fact_map_mutex);
    auto result = std::move(fact_map.at(timepoint));
    fact_map.erase(timepoint);
    return result;
}

void Reasoner::insert_conclusions(
    uint64_t timepoint,
    std::vector<std::shared_ptr<util::Grounding>> conclusions) {
    std::lock_guard<std::mutex> guard(conclusion_map_mutex);
    conclusion_map.try_emplace(timepoint, std::move(conclusions));
}

std::vector<std::shared_ptr<util::Grounding>>
Reasoner::get_conclusions(uint64_t timepoint) {
    std::lock_guard<std::mutex> guard(conclusion_map_mutex);
    auto result = std::move(conclusion_map.at(timepoint));
    conclusion_map.erase(timepoint);
    return result;
}

double Reasoner::start() {
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
    return clock_eval.count();
}

void Reasoner::read(util::Timeline timeline) {
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        auto facts = io_manager->read_stream_data(timeline);
        insert_facts(time, facts);
        timeline.increment_time();
        time = timeline.get_time();
    }
}

void Reasoner::evaluate(util::Timeline timeline) {
    auto program = Program(rule_reader);
    auto time = timeline.get_time();
    while (!timeline.is_past_max_time()) {
        bool has_new_input = fact_map.count(time) > 0;
        if (has_new_input) {
            auto facts = get_facts(time);
            auto clock_start = std::chrono::high_resolution_clock::now();
            auto conclusions = program.evaluate(timeline, std::move(facts));
            auto clock_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> clock_elapsed =
                clock_end - clock_start;
            clock_eval += clock_elapsed;
            insert_conclusions(time, std::move(conclusions));
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
            auto conclusions = get_conclusions(time);
            io_manager->write_output_data(time, std::move(conclusions));
            timeline.increment_time();
            time = timeline.get_time();
        } else {
            std::this_thread::yield();
        }
    }
}

} // namespace core
} // namespace laser
