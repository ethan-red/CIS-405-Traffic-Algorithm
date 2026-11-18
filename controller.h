#pragma once
#include <string>

using namespace std;

enum class Phase{ //Phases for each direction
    NS_GREEN, NS_YELLOW, ALL_RED1,
    EW_GREEN, EW_YELLOW, ALL_RED2
};

//Checks if a direction is in a specific state
inline bool is_green(Phase p){
    return p == Phase::NS_GREEN || p == Phase::EW_GREEN;
}
inline bool is_yellow(Phase p){
    return p == Phase::NS_YELLOW || p == Phase::EW_YELLOW;
}
inline bool is_all_red(Phase p){
    return p == Phase::ALL_RED1 || p == Phase::ALL_RED2;
}

struct IntersectionState{
    Phase phase = Phase::NS_GREEN;
    int ns_queue = 0;
    int ew_queue = 0;
    int ns_near = 0;
    int ew_near = 0;
    double time_in_phase = 0.0;
    double ns_red_time = 0.0;
    double ew_red_time = 0.0;
};

class Controller {
public:
    Phase update(const IntersectionState& state, double num);

    static constexpr double SWITCH_RATIO   = 1.3;
    static constexpr int    MIN_SWITCH_CARS = 2;
    static constexpr double MIN_GREEN      = 8.0;
    static constexpr double MAX_GREEN      = 45.0;
    static constexpr double MAX_WAIT       = 60.0;
};