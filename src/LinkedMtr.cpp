#include "./LinkedMtr.hpp";
#include "include.cpp";

LinkedMtr::LinkedMtr(const int count, const int mtr_ports[], bool altReverse)
{
    mtr_count = count;

    for (int i = 0; i < count; i++)
        motors.push_back(pros::Motor(mtr_ports[i], altReverse ? i % 2 == 0 : false));
};

void LinkedMtr::altReverse() {
    bool reverse = false;

    for (pros::Motor m : motors) {
        m.set_reversed(reverse);

        reverse = !reverse;
    }
}

void LinkedMtr::set_speed(int speed)
{
    for (pros::Motor m : motors)
        m = speed;
};

float LinkedMtr::get_actual_velocity() {
    float result = 0;

    for (pros::Motor m : motors)
        result += abs(m.get_actual_velocity());

    return result / mtr_count;
};
