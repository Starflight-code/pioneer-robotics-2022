#include "./MtrGroup.hpp"

MtrGroup::MtrGroup(const int count, const int mtr_ports[])
{
    mtr_count = count;

    for (int i = 0; i < count; i++)
        motors.push_back(pros::Motor(mtr_ports[i]));
};

void MtrGroup::alt_reverse() {
    bool reverse = false;

    for (pros::Motor m : motors) {
        m.set_reversed(reverse);

        reverse = !reverse;
    }
}

void MtrGroup::set_speed(int speed)
{
    for (pros::Motor m : motors)
        m = speed;
};

float MtrGroup::get_actual_velocity() {
    float result = 0;

    for (pros::Motor m : motors)
        result += abs(m.get_actual_velocity());

    return result / mtr_count;
};
