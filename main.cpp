#include <iostream>
#include "controller.h"
#include "simulation.h"
using namespace std;

int main(){
    SimConfig base;
    base.duration = 600.0; //10 simulated minutes

    Controller traffic_light;

    struct Scenario{
        const char* label;
        double ns, ew;
    };

    Scenario scenarios[] = {
        {"Balanced", 1.5, 1.5},
        {"NS-Heavy", 2.0, 0.5},
        {"EW-Heavy", 0.5, 2.0}
    };

    printf("%-10s %8s %8s %8s %8s\n", "scenario", "served", "avg_wait",
            "max_wait", "switches\n");
    for(auto& sc : scenarios){
        base.ns_arrival_rate = sc.ns;
        base.ew_arrival_rate = sc.ew;
        SimStats st = run_simulation(traffic_light, base);
        printf("%-10s %8lld %8.2f %8.2f %8d\n", sc.label,
            st.cars_passed, st.avg_wait(), st.max_wait, st.switches);
    }
    return 0;
}