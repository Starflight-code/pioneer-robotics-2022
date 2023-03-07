#ifndef pid_cpp_
#define pid_cpp_
#include "PID.cpp"
#endif

#ifndef algorithms_cpp_
#define algorithms_cpp_
#include "algorithms.cpp"
#endif

#ifndef include_cpp_
#define include_cpp_
#include "include.cpp"
#endif

/*
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "pros/vision.hpp"
*/

class cl {

private:
    double sc = 0;                       // Scaling constant, for use with arcade control system
    const int _SPINNER_SPEED = 60;       // Speed that spinner will spin at
    const int _SPINNER_DRIVE_SPEED = 10; // Speed for drive motors to push robot
                                         // forward, keeping contact with spinner
    bool spinnerActive = false;          // Allows for control while pushing robot forward
                                         // for spinner (Quality of Life)
    std::array<int, 2> controller_values;
    std::array<pros::controller_analog_e_t, 2> sticks;

public:
    Motor_Class Motors;
    algorithms algo;

private:
    void training() {
        pros::Controller master(pros::E_CONTROLLER_MASTER);
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            Motors.Robot.limiter = (double)(master.get_analog(ANALOG_LEFT_X) + algo._RANGE) / (algo._RANGE * 2);
        }
    }

public:
    /** Listens for certian events and acts on them
    Training: Left -> Sets limiter by checking full range left stick x axis value.
     * @return N/A
     */
    void event_listener() {
        if(Motors.Robot.training) {
            training();
        }
    }

    /** Control Listening Service
     *
     * Pulls from control inputs based on robot.cpp's config.
     * Sends calculated output to motors.
     * @return N/A
     */
    void controls() {

        pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
        // Set sticks arrays to correct values for current configuration
        if(Motors.Robot.controlScheme == 0) {
            sticks = {ANALOG_LEFT_Y, ANALOG_RIGHT_Y}; // Tank control
        } else {
            sticks = {ANALOG_LEFT_Y, ANALOG_RIGHT_X}; // Arcade Control
        }

        if(Motors.Robot.exponential_control) { // Apply exponential control altering and populate controller_values array
            for(int i = 0; i < controller_values.size(); i++) {
                controller_values[i] = algo.exponential_control(master.get_analog(sticks[i]), Motors.Robot.control_exponent_value);
            }
        } else {
            for(int i = 0; i < controller_values.size(); i++) { // Populate controller_values
                                                                // array with raw stick values
                controller_values[i] = master.get_analog(sticks[i]);
            }
        }

        Motors.leftMotors.set(controller_values[0]);
        Motors.rightMotors.set(controller_values[1]);

        switch(Motors.Robot.controlScheme) {
        case 0: // Tank Control
            for(int i = 0; i < controller_values.size(); i++) {
                Motors.Motors[i].set(algo.tank_control(controller_values[i], Motors.Robot.limiter));
            }
            break;
        case 1: // Split Arcade

            std::array<int, 2> motorValues;
            motorValues = algo.arcade_control(controller_values[0], controller_values[1], Motors.Robot.limiter);
            for(int i = 0; i < motorValues.size(); i++) {
                Motors.Motors[i].set(motorValues[i]);
            }
            //}
            break;
        default:
            // SHOULD NEVER OCCUR, but if it does...
            // There is an error in variables.cpp in
            // the Robot.controlScheme variable preset
            break;
        }
    }
};