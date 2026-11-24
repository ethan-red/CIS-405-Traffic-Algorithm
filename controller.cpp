#include "controller.h"
#include <algorithm>

using namespace std;

Phase Controller::update(const IntersectionState& state) {
    if (!is_green(state.phase)) return state.phase;
    bool ns_green = (state.phase == Phase::NS_GREEN);
    int current_queue = ns_green ? state.ns_queue : state.ew_queue;
    int other_queue   = ns_green ? state.ew_queue : state.ns_queue;

    double current_pressure = current_queue;
    double other_pressure   = other_queue;
    double red_wait = ns_green ? state.ew_red_time : state.ns_red_time;

    bool force_change    = red_wait >= MAX_WAIT;
    bool max_green_hit   = state.time_in_phase >= MAX_GREEN;
    bool pressure_change = other_pressure > current_pressure * SWITCH_RATIO
                           && other_queue >= MIN_SWITCH_CARS;
    bool min_green_passed = state.time_in_phase >= MIN_GREEN;

    bool should_switch = force_change
                       || (min_green_passed && (max_green_hit || pressure_change));

    if (should_switch)
        return ns_green ? Phase::NS_YELLOW : Phase::EW_YELLOW;

    return state.phase;
}