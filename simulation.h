#pragma once
#include "controller.h"
#include <string>

struct SimConfig{
    double ns_arrival_rate = 0.25; //Default NS arrival rate
    double ew_arrival_rate = 0.25; //Default EW arrival rate
    double num = 0.1;
    double duration = 600.0; //10 simulated minutes
    double yellow_time = 5.0; //Light is yellow for 5 simulated seconds
    double all_red_time = 2.0; //Both lights are red for 2 simulated seconds
    int print_rate = 600.0; //How many ticks between stat outputs (tick = 0.1 sec)
    int discharge_rate = 3; //How fast the intersection discharges vehicles (3 per 0.1 sec)
};

struct SimStats{
    long long cars_arrived = 0;
    long long cars_passed = 0;
    double total_wait = 0.0;
    double max_wait = 0.0;
    int switches = 0; //How many phase switches the traffic light makes
    double total_time = 0.0;
    double avg_wait() {
        if(cars_arrived == 0){
            return 0.0;
        }
        return total_wait/cars_arrived;
    }
};

SimStats run_simulation(Controller& ctrl, const SimConfig& config);