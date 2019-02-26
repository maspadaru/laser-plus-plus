//
// Created by mike on 02/25/19.
//
#ifndef LASER_REASONER_SEQUENTIAL_REASONER_H
#define LASER_REASONER_SEQUENTIAL_REASONER_H

namespace laser {
namespace reasoner {

class SequentialReasoner {
  private:
    std::unordered_map<uint64_t, std::vector<io::DataAtom>> fact_map;
    std::unordered_map<uint64_t, std::vector<io::DataAtom>> conclusion_map;

    laser::io::IOManager *ioManager;

    void read(Timeline timeline);
    void evaluate(Timeline timeline);
    void write(Timeline timeline);

  public:

    SequentialReasoner(std::string rule_string, laser::io::IOManager *ioManager);
    SequentialReasoner(laser::rule::RuleReader *rule_reader,
            laser::io::IOManager *ioManager);

    virtual ~SequentialReasoner();

    void start();

}

} // namespace reasoner 
} // namespace laser
#endif // LASER_REASONER_SEQUENTIAL_REASONER_H
