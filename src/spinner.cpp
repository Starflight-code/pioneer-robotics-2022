/// Allows interaction with color sensor and running automated spinner systems
#ifndef include_cpp_
#include "include.cpp"
#define include_cpp_
#endif

/// Allows control and centralized tracking of spinner system
class spin {
private:
    int spin_color;

public:
    /** Gets the current color from the specified color sensor
     * @param port | port of the color sensor (integer w/ range [1 <-> 21])
     * @return color detected, 1 for red and 2 for blue
     */
    int get_color(int port) {
        pros::Optical vis(port);
        pros::c::optical_rgb_s rgb = vis.get_rgb();
        // Branchless method to check colors, and return 1 for red or 2 for blue
        int color = (rgb.blue < 40) * (rgb.green < 40) * (rgb.red > 160) * 1 + (rgb.red < 40) * (rgb.green < 40) * (rgb.blue > 160) * 2;
        color = color != 0 ? color : -1; // Checks if the color is undefined, if so -1 is returned
        // Include color sensor code, to convert values to red: 1 or blue: 2
        return color;
    };
    /** Rotates motor to a defined color by using a sensor
     * @param sensorPort | port of the color sensor (integer w/ range [1 <-> 21])
     * @param targetColor | color to move the motor to. 1 (red) or 2 (blue) (integer w/ range [1 <-> 2])
     * @param motorPort | port of the target motor (integer w/ range [1 <-> 21])
     * @return N/A
     */
    void rotate(int sensorPort, int targetColor, int motorPort) {
        // Inlude code to rotate the motor and stop on the correct color
        pros::Motor motor(motorPort);
        while(get_color(sensorPort) != targetColor) {
            motor = 20;
            pros::delay(50);
        }
    }
    /** Rotates motor by a certian amount of encoder units
     * @param motorPort | port of the target motor (integer w/ range [1 <-> 21])
     * @param encoderUnits | encoder units to spin motor, 1.2 degrees per encoder unit (integer)
     * @return N/A
     */
    void rotate(int motorPort, int encoderUnits) {
        pros::Motor motor(motorPort);
        motor.tare_position();
        motor = 15;
        while(motor.get_position() < 300) { // 300 encoder units in 360 degrees (1.2 degrees per encoder unit)
            pros::delay(10);
        }
        motor = 0;
    };
};