#include "simulation.h"
#include <queue>
#include <vector>
#include <cstdio>

using namespace std;

SimStats run_simulation(Controller& ctrl, const SimConfig& config){
    SimStats stats;

    Phase phase = Phase::NS_GREEN;
    double phase_time = 0.0;
    double ns_red = 0.0, ew_red = 0.0;

    vector <double> ns_q, ew_q;
    double ns_accum = 0.0, ew_accum = 0.0;

    long long tick = 0;
    double time = 0.0;
    double discharge_accum = 0.0;

    while(time < config.duration){
        ns_accum += config.ns_arrival_rate * config.num;
        while(ns_accum >= 1.0){
            ns_q.push_back(time);
            ++stats.cars_arrived;
            ns_accum -= 1;
        }

        ew_accum += config.ew_arrival_rate * config.num;
        while(ew_accum >= 1.0){
            ew_q.push_back(time);
            ++stats.cars_arrived;
            ew_accum -= 1;
        }

        if(phase == Phase::NS_GREEN){
            discharge_accum += config.discharge_rate * config.num;
            while(discharge_accum >= 1.0 && !ns_q.empty()){
                double arrived = ns_q.front(); ns_q.erase(ns_q.begin());
                double wait = time - arrived;
                stats.total_wait += wait;
                stats.max_wait = max(stats.max_wait, wait);
                ++stats.cars_passed;
                discharge_accum -= 1.0;
            }
        }
        else if(phase == Phase::EW_GREEN){
            discharge_accum += config.discharge_rate * config.num;
            while(discharge_accum >= 1.0 && !ew_q.empty()){
                double arrived = ew_q.front(); ew_q.erase(ew_q.begin());
                double wait = time - arrived;
                stats.total_wait += wait;
                stats.max_wait = max(stats.max_wait, wait);
                ++stats.cars_passed;
                discharge_accum -= 1.0;
            }
        }
        else{discharge_accum = 0.0;}

        if(phase == Phase::NS_GREEN){ns_red = 0.0;}
        else{ns_red += config.num;}

        if(phase == Phase::EW_GREEN){ew_red = 0.0;}
        else{ew_red += config.num;}

        IntersectionState s;
        s.phase = phase;
        s.ns_queue = (int)ns_q.size();
        s.ew_queue = (int)ew_q.size();
        s.ns_near = s.ns_queue;
        s.ew_near = s.ew_queue;
        s.time_in_phase = phase_time;
        s.ns_red_time = ns_red;
        s.ew_red_time = ew_red;

        Phase requested = ctrl.update(s, config.num);

        if(is_green(s.phase) && is_yellow(requested)){
            s.phase = requested;
            phase_time = 0.0;
            ++stats.switches;
        }
        else if(is_yellow(s.phase) && phase_time >= config.yellow_time){
            s.phase = (s.phase == Phase::NS_YELLOW) ? Phase::ALL_RED1 : Phase::ALL_RED2;
            phase_time = 0.0;
        }
        else if(is_all_red(s.phase) && phase_time >= config.all_red_time){
            s.phase = (s.phase == Phase::ALL_RED1) ? Phase::EW_GREEN : Phase::NS_GREEN;
            phase_time = 0.0;
        }

        if(tick % 1000 == 0){
            printf("time=%6.1f phase=%d NS_q=%2zu EW_q=%2zu served=%4lld avg_wait=%6.2f\n",
               time, s.phase, ns_q.size(), ew_q.size(),
               stats.cars_passed, stats.avg_wait());
        }
        ++tick;

        phase_time += config.num;
        time += config.num;
        stats.total_time = time;
    }
    return stats;
}