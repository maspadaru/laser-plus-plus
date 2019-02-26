//
// Created by mike on 02/25/19.
//

#include "reasoner/sequential_reasoner.h"

namespace laser {
namespace reasoner {

void SequentialReasoner::start() {
    // create program

        read();
        evaluate();
        write();


}

void SequentialReasoner::evaluate() {
    program = new Program()
    while (!program.is_done()) {
        program.evaluate();
    }

}
    
void SequentialReasoner::read() {
    while()
    auto facts = ioHandler.get_stream_data(timeline);

}

void SequentialReasoner::write() {

}

} // namespace reasoner 
} // namespace laser
