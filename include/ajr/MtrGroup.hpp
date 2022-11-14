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

    // --------------------------------------------------
    // Alternative methods for those that would appear on
    // A normal pros::Motor
    // --------------------------------------------------

    /**
     * Sets the voltage of each motor
     *
     * @param voltage - The new motor voltage from -127 to 127
     */
    void set_speed(int voltage);

    /**
     * Sets each motor in the reversed value
     * 
     * @param reversed True reverses every motor, false motor direction is default
     */
    void set_reversed(const bool reversed);

    /**
     * Gets the average actual velocity of the motors
     *
     * NOTE: Takes the absolute value of each motors velocity before adding it
     *
     * @return The average actual velocity of the motors
     */
    float get_actual_velocity();
};
