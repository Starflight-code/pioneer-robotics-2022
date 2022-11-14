#include "pros/motors.hpp"

class MtrGroup
{
private:
    int mtr_count;
    std::vector<pros::Motor> motors;

public:
    /**
     * Default constructor for weired c++ things
     */
    MtrGroup();

    /**
     * Create MtrGroup - Motor Group from parameters
     * @param count the number of motors in group
     * @param mtr_ports array of motor ports
     */
    MtrGroup(const int count, const int mtr_ports[]);

    /**
     * Alternates by setting every other motor in motors
     * to reverse. Starting with `true`.
     */
    void alt_reverse();

    void set_speed(int speed);
    float get_actual_velocity();
};
