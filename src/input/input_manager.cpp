//
// Created by mike on 6/17/18.
//

#include <formula/extended/atom.h>
#include "input/input_manager.h"

namespace laser {
namespace input {

// getters & setters

uint64_t InputManager::get_stream_start_time() const {
    return stream_start_time;
}

uint64_t InputManager::get_stream_end_time() const {
    return stream_end_time;
}

// ====================== Stream Reader ======================================

bool InputManager::is_initialised_stream_reader() const {
    return is_initialised_stream_reader_m;
}


void InputManager::initialize_stream_reader(
        DataReader *stream_data_reader,
        DataParser *stream_data_parser) {
    this->stream_data_reader = stream_data_reader;
    this->stream_data_parser = stream_data_parser;
    this->is_initialised_stream_reader_m = true;
}

bool InputManager::fetch_stream_metadata() {
    if (!is_initialised_stream_reader_m) {
        throw exception::UninitializedException("The background data reader "
                                                "was not initialised. Call "
                                                "initialize_stream_reader() "
                                                "before calling "
                                                "get_stream_facts()");
    }
    if (!has_metadata) {
        if (this->stream_data_reader->fetch_metadata()) {
            this->stream_start_time =
                    this->stream_data_reader->get_stream_start_time();
            this->stream_end_time =
                    this->stream_data_reader->get_stream_end_time();
            this->has_metadata = true;
        }
    }
    return has_metadata;
}

std::tuple<uint64_t, uint64_t,
        std::unordered_map<std::string, std::vector<formula::Formula *>>>
InputManager::get_stream_facts(uint64_t request_time_point) {
    if (!is_initialised_stream_reader_m) {
        throw exception::UninitializedException("The background data reader "
                                                "was not initialised. Call "
                                                "initialize_stream_reader() "
                                                "before calling "
                                                "get_stream_facts()");
    }
    if (request_time_point != stream_current_time + 1) {
        throw exception::RequestException("the requested time point is"
                                          "not equal to the next time point of "
                                          "the stream");
    }

    int time_point_of_facts;
    std::vector<std::string> raw_stream_facts;
    std::tie(time_point_of_facts, raw_stream_facts)
            = stream_data_reader->read_next_data(request_time_point);
    stream_current_time++;
    if (time_point_of_facts != stream_current_time) {
        throw exception::RequestException("the requested time point could"
                                          "not be read from the stream ");
    }
    int number_of_facts;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            parsed_stream_facts_map;
    auto pseudo_formula_vector
            = stream_data_parser->parse_data(raw_stream_facts);

    if (!pseudo_formula_vector.empty()) {
        parsed_stream_facts_map = make_formula_map(pseudo_formula_vector);
        // stream_tuple_counter += pseudo_formula_vector.size();
    }


    return std::make_tuple(this->stream_current_time,
            this->stream_tuple_counter,
            parsed_stream_facts_map);

}

laser::formula::Formula *
InputManager::build_data_formula(formula::PseudoFormula pseudo_formula) {
    laser::formula::Formula *result = nullptr;
    switch (pseudo_formula.type) {
        case laser::formula::FormulaType::ATOM : {
            auto atom = new laser::formula::Atom(pseudo_formula.predicate);
            // TODO add groundings: pseudo_formula.argument_vector
            result = atom;
            break;
        }
        case formula::FormulaType::CONJUNCTION:break;
        case formula::FormulaType::DISJUNCTION:break;
        case formula::FormulaType::IMPLICATION:break;
        case formula::FormulaType::NEGATION:break;
        case formula::FormulaType::EXACT_TIME:break;
        case formula::FormulaType::TUPLE_WINDOW:break;
        case formula::FormulaType::TIME_WINDOW:break;
        case formula::FormulaType::BOX:break;
        case formula::FormulaType::DIAMOND:break;
    }
    return result;
}

laser::formula::Formula *InputManager::find_or_add_formula(
        formula::PseudoFormula pseudo_formula,
        std::vector<laser::formula::Formula *> *formula_vector) {
    for (auto *formula : *formula_vector) {
        if (formula->get_type() == pseudo_formula.type) {
            return formula;
        }
    }
    auto result = build_data_formula(pseudo_formula);
    formula_vector->push_back(result);
    return result;
}

std::unordered_map<std::string, std::vector<formula::Formula *>>
InputManager::make_formula_map(
        std::vector<formula::PseudoFormula> pseudo_formulas) {
    std::unordered_map<std::string,
            std::vector<laser::formula::Formula * >> formula_map;
    uint64_t formula_count = stream_tuple_counter;
    stream_tuple_counter++;
    uint64_t consideration_time = stream_current_time;
    uint64_t horizon_time = stream_current_time;
    uint64_t horizon_count = ULLONG_MAX;

    for (const auto &pseudo_formula : pseudo_formulas) {
        formula_count++;
        uint64_t consideration_count = formula_count;
        // Vector of facts should have only one formula of each type, so the
        // list is short and iterable.
        // Once I found a matching FormulaType, I add a new substitution to it
        // if I don't find it, then I create one using build_data_formula
        formula_map.try_emplace(pseudo_formula.predicate);
        std::vector<laser::formula::Formula *>
                &map_value = formula_map[pseudo_formula.predicate];
        laser::formula::Formula *formula =
                find_or_add_formula(pseudo_formula, &map_value);
        formula->add_grounding(consideration_time, horizon_time,
                consideration_count, horizon_count, pseudo_formula.arguments);
    }
    return formula_map;
}

// ====================== Background Reader ====================================


bool InputManager::is_initialised_background_reader() const {
    return is_initialised_background_reader_m;
}

void
InputManager::initialize_background_reader(
        DataReader *background_data_reader,
        DataParser *background_data_parser) {
    this->background_data_reader = background_data_reader;
    this->background_data_parser = background_data_parser;
    this->is_initialised_background_reader_m = true;
}


std::tuple<uint64_t,
        std::unordered_map<std::string, std::vector<formula::Formula *>>>
InputManager::get_background_facts() {
    if (!is_initialised_background_reader_m) {
        throw exception::UninitializedException("The background data reader "
                                                "was not initialised. "
                                                "Call initialize_background_reader() "
                                                "before calling get_background_facts()");
    }
    auto raw_background_facts = background_data_reader->read_all_data();
    uint64_t number_of_parsed_facts;
    std::unordered_map<std::string, std::vector<formula::Formula *>>
            parsed_background_facts_map;
    auto pseudo_formula_vector
            = stream_data_parser->parse_data(raw_background_facts);
    parsed_background_facts_map = make_formula_map(pseudo_formula_vector);
    number_of_parsed_facts = pseudo_formula_vector.size();
    return std::make_tuple(number_of_parsed_facts,
            parsed_background_facts_map);
}

// ========================== Rule Reader ======================================

bool InputManager::is_initialised_rule_reader() const {
    return is_initialised_rule_reader_m;
}

void InputManager::initialize_rule_reader(
        RuleReader *rule_reader,
        RuleParser *rule_parser) {
    this->rule_reader = rule_reader;
    this->rule_parser = rule_parser;
    this->is_initialised_rule_reader_m = true;
}

std::vector<rule::Rule> InputManager::get_rules() {
    if (!is_initialised_rule_reader_m) {
        throw exception::UninitializedException("The rule reader was not "
                                                "initialised. Call "
                                                "initialize_rule_reader() "
                                                "before calling get_rules()");
    }
    auto raw_rules = rule_reader->read_rules();
    auto pseudo_rule_vector = rule_parser->parse_rules(raw_rules);
    auto rule_vector = make_rule_vector(pseudo_rule_vector);
    return rule_vector;
}

std::vector<rule::Rule> InputManager::make_rule_vector(
        std::vector<std::tuple<formula::PseudoFormula,
                std::vector<formula::PseudoFormula>>>
        pseudo_rule_vector) {
    std::vector<rule::Rule> rule_vector;
    for (auto const &tuple : pseudo_rule_vector) {
        formula::PseudoFormula rule_head;
        std::vector<formula::PseudoFormula> rule_body_vector;
        std::tie(rule_head, rule_body_vector) = tuple;
        if (rule_head.predicate.empty()) {
            // TODO: Can this really be empty? Would the rule be useful at all?
            continue;
        }
        if (rule_body_vector.empty()) {
            // TODO: Can a rule have a head but no body?
            continue;
        }
        rule::Rule rule = build_rule(rule_head, rule_body_vector);
        rule_vector.push_back(rule);
    }
    return rule_vector;
}

laser::formula::Formula *
InputManager::build_rule_formula(formula::PseudoFormula pseudo_formula) {
    laser::formula::Formula *result = nullptr;
    switch (pseudo_formula.type) {
        case laser::formula::FormulaType::ATOM : {
            auto atom = new laser::formula::Atom(pseudo_formula.predicate,
                    pseudo_formula.arguments);
            result = atom;
            break;
        }
        case formula::FormulaType::CONJUNCTION:break;
        case formula::FormulaType::DISJUNCTION:break;
        case formula::FormulaType::IMPLICATION:break;
        case formula::FormulaType::NEGATION:break;
        case formula::FormulaType::EXACT_TIME:break;
        case formula::FormulaType::TUPLE_WINDOW:break;
        case formula::FormulaType::TIME_WINDOW:break;
        case formula::FormulaType::BOX:break;
        case formula::FormulaType::DIAMOND:break;
    }
    return result;
}

laser::rule::Rule InputManager::build_rule(
        formula::PseudoFormula pseudo_head,
        std::vector<formula::PseudoFormula> pseudo_body) {
    // TODO In the python version they reverse(body). Does the order matter?
    laser::formula::Formula *head = build_rule_formula(std::move(pseudo_head));
    std::vector<laser::formula::Formula *> body;
    for (const auto &pseudo_formula : pseudo_body) {
        body.push_back(build_rule_formula(pseudo_formula));
    }
    auto result = laser::rule::Rule(head, body);
    // freeing allocations:
    delete head;
    for (auto formula : body) {
        delete formula;
    }
    body.clear();
    return result;
}


} // namespace input
} // namespace laser
