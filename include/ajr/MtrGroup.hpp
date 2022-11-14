// #include "pros/motors.hpp"
#include "../main.h"

class MtrGroup
{
private:
    int mtr_count;
    std::vector<pros::Motor> motors;

public:
    MtrGroup();
    MtrGroup(const int count, const int mtr_ports[]);

    void alt_reverse();

    void set_speed(int speed);
    float get_actual_velocity();    
};
