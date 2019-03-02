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
    read_secs = 0;
    evaluate_secs = 0;
    write_secs = 0;
    util::Timeline main_timeline;
    auto program = program::Program(rule_reader);
    auto start_time = io_manager->read_stream_start_time();
    auto end_time = io_manager->read_stream_end_time();
    main_timeline.set_start_time(start_time);
    main_timeline.set_min_time(start_time);
    main_timeline.set_max_time(end_time);
    while (!main_timeline.is_past_max_time()) {
        read(main_timeline);
        evaluate(main_timeline, program);
        write(main_timeline);
        main_timeline.increment_time();
    }
    std::cout << "Read / Eval / Write  " << read_secs 
       << " / " << evaluate_secs << " / " << write_secs << std::endl;
}

void SequentialReasoner::read(util::Timeline timeline) {
    clock_t begin = clock();
    auto time = timeline.get_time();
    facts = io_manager->read_stream_data(time);
    clock_t end = clock();
    read_secs += double(end - begin) / CLOCKS_PER_SEC;
}

void SequentialReasoner::evaluate(util::Timeline timeline,
                                  program::Program &program) {
    clock_t begin = clock();
    auto time = timeline.get_time();
    conclusions = program.evaluate(timeline, facts);
    clock_t end = clock();
    evaluate_secs += double(end - begin) / CLOCKS_PER_SEC;
}

void SequentialReasoner::write(util::Timeline timeline) {
    clock_t begin = clock();
    auto time = timeline.get_time();
    io_manager->write_output_data(time, conclusions);
    clock_t end = clock();
    write_secs += double(end - begin) / CLOCKS_PER_SEC;
}

} // namespace reasoner
} // namespace laser
