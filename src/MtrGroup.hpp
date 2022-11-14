#include "include.cpp"

class MtrGroup
{
private:
    int mtr_count;
    std::vector<pros::Motor> motors;

public:
    MtrGroup();
    MtrGroup(const int mtr_count, const int mtr_ports[]);

    /**
     * Sets the speed of all the motors in the group
     */
    void set_speed(int speed);
    /**
     * Sets every other motor in group to reverse. Starting with the first one.
     */
    void alt_reverse();
    /**
     * Gets the actual velocity of all motors in groups.
     * 
     * Note: applies absolute value to all veleocities before
     * all are added
     */
    float get_actual_velocity();
};
