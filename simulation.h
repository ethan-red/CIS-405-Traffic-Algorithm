#include "controller.h"

struct SimConfig{
    double ns_arrival_rate = 0.25;
    double ew_arrival_rate = 0.25;
    double num = 0.1;
    double duration = 600.0;
    double yellow_time = 5.0;
    double all_red_time = 2.0;
    int discharge_rate = 1;
    int near_intersection = 6;

    unsigned seed = 42;
};

struct SimStats{
    long long cars_arrived = 0;
    long long cars_passed = 0;
    double total_wait = 0.0;
    double max_wait = 0.0;
    int switches = 0;
    double total_time = 0.0;
    double avg_wait() const{
        if(cars_passed == 0){
            return 0.0;
        }
        return total_wait/cars_passed;
    }
}

SimStats run_simulation(Controller& ctrl, const SimConfig& config, bool trace = false);