#include <string>
using namespace std;

enum class Phase{
    NS_GREEN, NS_YELLOW, NS_RED,
    EW_GREEN, EW_YELLOW, EW_RED
};

bool is_green(Phase p){
    return p == Phase::NS_GREEN || p == Phase::EW_GREEN;
}
bool is_yellow(Phase p){
    return p == Phase::NS_YELLOW || p == Phase::EW_YELLOW;
}
bool is_red(Phase p){
    return p == Phase::NS_RED || p == Phase::EW_RED;
}

struct IntersectionState{
    int ns_queue = 0;
    int ew_queue = 0;
    int ns_near = 0;
    int ew_near = 0;
    double time_in_phase = 0.0;
    double ns_red_time = 0.0;
    double ew_red_time = 0.0;
};

class Controller{
    virtual ~Controller() = default;
    virtual Phase update(const IntersectionState& state, double num);
    virtual string name() const = 0;
}