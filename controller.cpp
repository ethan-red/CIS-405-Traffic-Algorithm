#include "controller.h"
#include <algorithm>

using namespace std;

class GController : public Controller{ //Greedily chooses
public:
    Phase update(const IntersectionState& state, double) override {
        bool ns_green = (current == Phase::NS_GREEN);
        int current_direction = ns_green ? state.ns_queue : state.ew_queue;
        int other_direction = ns_green ? state.ew_queue : state.ns_queue;
        if(current_direction == 0 && other_direction > 0){return next_yellow();}
        return current;
    }
    string name() const override{return "Greedy";}

private:
    Phase current = Phase::NS_GREEN;
    Phase next_yellow(){
        if(current == Phase::NS_GREEN) {current = Phase::NS_YELLOW;}
        else {current = Phase::EW_YELLOW;}

        return current;
    }
};

class TController : public Controller{ //Fairly chooses
public:
    static constexpr double SWITCH_RATIO = 1.3;
    static constexpr int MIN_SWITCH_CARS = 2;
    static constexpr double MIN_GREEN = 8.0;
    static constexpr double MAX_GREEN = 45.0;
    static constexpr double MAX_WAIT = 60.0;
    Phase update(const IntersectionState& state, double) override{
        if(!is_green(current)) return current;

        bool ns_green = (current == Phase::NS_GREEN);
        /*Example: If North/South is Green, current_queue = the North/South queue
                   If North/South is not Green, current_queue = the East/West queue*/         
        int current_queue = ns_green ? state.ns_queue : state.ew_queue;
        int other_queue = ns_green ? state.ew_queue : state.ns_queue;
        int current_near = ns_green ? state.ns_near : state.ew_near;
        int other_near = ns_green ? state.ew_near : state.ns_near;

        double current_pressure = current_queue + 0.25 * current_near;
        //Note: Some cars may take a right turn in queue, so this queue can also fluctuate
        double other_pressure = other_queue + 0.25 * other_near;
        double red_wait = ns_green ? state.ew_red_time : state.ns_red_time;

        bool force_change = red_wait >= MAX_WAIT;
        bool max_green_hit = state.time_in_phase >= MAX_GREEN;
        bool pressure_change = other_pressure > current_pressure * SWITCH_RATIO
                                && other_queue >= MIN_SWITCH_CARS;
        bool min_green_passed = state.time_in_phase >= MIN_GREEN;

        /*Switches if max_wait time has passed, or if the pressure from the other direction
        is greater than the current direction with a green light*/
        bool should_switch = force_change || (min_green_passed && max_green_hit
                                || pressure_change);
        
        if(should_switch){
            current = ns_green ? Phase::NS_YELLOW : Phase::EW_YELLOW;
        }
        return current;
    }
    string name() const override {return "Threshold";}
    void notify_phase(Phase p) {current = p;}

private:
    Phase current = Phase::NS_GREEN;
};