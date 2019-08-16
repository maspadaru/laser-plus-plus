#include "rule/math_atom.h"

namespace laser::rule {

std::vector<std::shared_ptr<util::Grounding>>
MathAtom::evaluate(std::vector<std::shared_ptr<util::Grounding>> body_facts) {

    //auto left_index = body.get_variable_index(left_name);
    //if (left_index > 0) {
        //for (auto &body_fact: body_facts) {
            //auto body_constant = body_fact.get_constant(left_index);
            //int64_t left_value = strtoll(body_constant, NULL, 0); 
            //int64_t result = left_value + right_value;
            ////add variable to groundig on the positon of rule::trigger_variables

        //}

    //}


    return body_facts;
}

std::string const & MathAtom::get_result_name() const {
    return result_name;
}

} // namespace laser::rule
