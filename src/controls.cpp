#ifndef pid_cpp_
#define pid_cpp_
#include "PID.cpp"
#include "pros/misc.h"
#include "pros/rtos.h"
#include <cmath>
#endif

#ifndef algorithms_cpp_
#define algorithms_cpp_
#include "algorithms.cpp"
#endif

#ifndef include_cpp_
#define include_cpp_
#include "include.cpp"
#endif

/**
 * Tracks the status of a toggle switch
 */
class ToggleTracker {
private:
    bool held;
    bool previousState;

public:
    bool currentState;
    bool modifed;

    /** Initialzes the toggle tracker.
     * @param state the initial toggle state (boolean)
     */
    ToggleTracker() {
        held = false;
        currentState = false;
        previousState = currentState;
    }

    /** Initialzes the toggle tracker.
     * @param state the initial toggle state (boolean)
     */
    ToggleTracker(bool state) {
        held = false;
        currentState = state;
        previousState = currentState;
    }

    /** Updates the tracker with the current button value
     *  @param held current value of the button (boolean)
     */
    void updateTracker(bool held) {
        if(held && held != previousState) {
            currentState = !currentState;
            modifed = true;
        } else {
            modifed = false;
        }
        previousState = held;
    }
};

class cl {

private:
    double sc = 0;      // Scaling constant, for use with arcade control system
    bool spinnerActive; // Tracks if the spinner is on or off
    std::array<int, 2> controller_values;
    std::array<pros::controller_analog_e_t, 2> sticks;

public:
    Motor_Class Motors;
    Algorithms algo;
    // Control_Algorithms pidOne();
    // Control_Algorithms pidTwo();
    ToggleTracker swapControls;
    ToggleTracker pistonLauncher;
    ToggleTracker launcherTracker;

private:
    /** A listener that runs every 50 ms and runs when training mode is on
     * @return N/A
     */
    void training() {
        pros::Controller master(pros::E_CONTROLLER_MASTER);
        if(master.get_digital(Motors.preset.controlButtons[0])) {

            // sets the local limiter, using the full range of the x axis on the left controller stick
            Motors.preset.limiter = (double)(master.get_analog(ANALOG_LEFT_X) + algo._RANGE) / (algo._RANGE * 2);
        }
    }

public:
    /** Listens for certian events and acts on them
    Training: Left -> Sets limiter by checking full range left stick x axis value.
     * @return N/A
     */
    void event_listener() {
        pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"

        if(Motors.preset.training) {
            training(); // loads training listener
        }

        if(launcherTracker.modifed) { // starts a position movement, only when the toggle button is pressed (executes once per button press)
            Motors.launcherMotors.setPosition(Motors.preset.launcherAutoPullbackSpeed, Motors.preset.launcherRunLength);
        }

        // updates the toggle trackers with the updated digital input values
        swapControls.updateTracker(master.get_digital(Motors.preset.controlButtons[4]));
        pistonLauncher.updateTracker(master.get_digital(Motors.preset.controlButtons[1]));
        launcherTracker.updateTracker(master.get_digital(Motors.preset.controlButtons[5]));

        // sets the piston to the toggle tracker's output
        Motors.stringLauncher.set(pistonLauncher.currentState);

        if(master.get_digital(Motors.preset.controlButtons[7])) { // Starts the intake if button is pressed and held
            Motors.intakeMotors.set(Motors.preset.intakeSpeed);
        } else {
            Motors.intakeMotors.set(0);
        }

        if(master.get_digital(Motors.preset.controlButtons[6])) { // pulls the motor back at a pre-defined speed when button is pressed
                                                                  // only when an auto pullback is not running
            Motors.launcherMotors.set(Motors.preset.launcherManualPullbackSpeed);
        } else if(not master.get_digital(Motors.preset.controlButtons[6]) && not Motors.launcherMotors.positionCheckStatus()) {
            Motors.launcherMotors.set(0);
        }

        if(master.get_digital(Motors.preset.controlButtons[2])) { // Spinner Normal Direction
            Motors.spinnerMotors.set(Motors.preset.spinner_speed * -1);
            spinnerActive = true;
        } else if(master.get_digital(Motors.preset.controlButtons[3])) { // Spinner Reversed Direction
            Motors.spinnerMotors.set(Motors.preset.spinner_speed);
            spinnerActive = true;
        } else {
            Motors.spinnerMotors.set(0);
            spinnerActive = false;
        }

        // checks if the target position is reached for an active automatic motion of the launcher mechanism
        Motors.launcherMotors.checkPosition();
    }

    /** Control Listening Service
     *
     * Pulls from control inputs based on robot.cpp's config.
     * Sends calculated output to motors.
     * @return N/A
     */
    void
    controls() {
        // Control_Algorithms pidOne(0.2, 0.04, 0.01);
        // Control_Algorithms pidTwo(0.2, 0.04, 0.01);

        pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"

        // Set sticks arrays to correct values for current configuration
        if(Motors.preset.controlScheme == Robot::Tank) {
            sticks = {ANALOG_LEFT_Y, ANALOG_RIGHT_Y}; // Tank control
        } else {
            sticks = {ANALOG_LEFT_Y, ANALOG_RIGHT_X}; // Arcade Control
        }

        if(Motors.preset.exponential_control) { // Apply exponential control altering and populate controller_values array
            for(int i = 0; i < controller_values.size(); i++) {
                controller_values[i] = algo.exponential_control(master.get_analog(sticks[i]), Motors.preset.control_exponent_value);
            }
        } else {
            for(int i = 0; i < controller_values.size(); i++) { // Populate controller_values
                                                                // array with raw stick values
                controller_values[i] = master.get_analog(sticks[i]);
            }
        }

        switch(Motors.preset.controlScheme) {
        case Robot::Tank: // populates the controller_values array with calculated values specifically for tank drive
            for(int i = 0; i < controller_values.size(); i++) {
                controller_values[i] = algo.tank_control(controller_values[i], Motors.preset.limiter);
            }
            break;
        case Robot::Arcade: // populates the controller_values array with calculated values specifically for arcade control
            controller_values = algo.arcade_control(controller_values[0], controller_values[1], Motors.preset.limiter);
            break;
        default:
            // SHOULD NEVER OCCUR, but if it does...
            // There is an error in variables.cpp in
            // the Robot.controlScheme variable preset
            break;
        }
        // Applys motor speeds from controller_values array
        if(not swapControls.currentState) {
            controller_values = algo.controlSwap(controller_values);
        }

        // applies the L_R_Offset override if applicable, updating array
        controller_values = algo.applyOffset(controller_values[0], controller_values[1], Motors.preset.left_right_motor_offset);

        // sends array values to each motor group
        Motors.leftMotors.set(controller_values[0] + (spinnerActive * Motors.preset.spinner_boost));
        Motors.rightMotors.set(controller_values[1] + (spinnerActive * Motors.preset.spinner_boost));
    }
};