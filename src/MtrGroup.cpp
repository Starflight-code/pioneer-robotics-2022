#include "ajr/MtrGroup.hpp"

MtrGroup::MtrGroup() {}

MtrGroup::MtrGroup(const int count, const int mtr_ports[])
{
    mtr_count = count;

    for (int i = 0; i < count; i++)
        motors.push_back(pros::Motor(mtr_ports[i]));
}

void MtrGroup::alt_reverse()
{
    bool reversed = true;

    for (pros::Motor m : motors)
    {
        m.set_reversed(reversed);

        reversed = !reversed;
    }
}

void MtrGroup::set_speed(int speed)
{
    for (pros::Motor m : motors)
        m = speed;
}

void MtrGroup::set_reversed(const bool reversed)
{
    for (pros::Motor m : motors)
        m.set_reversed(reversed);
}

float MtrGroup::get_actual_velocity()
{
    float result = 0;

    for (pros::Motor m : motors)
        result += m.get_actual_velocity();

    return result / mtr_count;
}
