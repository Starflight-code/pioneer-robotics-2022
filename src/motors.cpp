#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <cmath>
#include <sys/types.h>

#ifndef main_h_
#include "main.h"
#define main_h_
#endif

#ifndef robot_cpp_
#include "robot.cpp"
#define robot_cpp_
#endif

/**
 * Tracks, stores and moves a piston after initialized
 */
class Piston { // Piston class, supports ADI Pistons and includes tracking/setting
private:
    std::vector<pros::ADIDigitalOut> piston = {};
    bool piston_state; // Allows for toggle/state fetching
public:
    /** Initializes the class, using the piston port. Syncing the state to retracted by default.
     * @param ADI_Port port for the piston (integer w/ range [1 <-> 8])
     * @param initial_state initial state to sync piston to (bool, optional)
     */
    void init(int ADI_Port) { // Initializes the piston, this needs to be completed before it can be used
        piston.push_back(pros::ADIDigitalOut(ADI_Port));
        piston[0].set_value(false); // Retracts the piston, syncing it's state
        piston_state = false;       // Sets the piston state, allowing for tracking
    }

    /** Initializes the class, using the piston port. Syncing the state to retracted by default.
     * @param ADI_Port port for the piston (integer w/ range [1 <-> 8])
     * @param initial_state initial state to sync piston to (bool, optional)
     */

    void init(int ADI_Port, bool initial_state) { // Initializes the piston, this needs to be completed before it can be used
        piston.push_back(pros::ADIDigitalOut(ADI_Port));
        piston[0].set_value(initial_state); // Retracts the piston, syncing it's state
        piston_state = initial_state;       // Sets the piston state, allowing for tracking
    }

    /** Sets the piston to false (retracted) or true (extended), updates tracker
     * @param state false (retracted) or true (extended), sets piston state (bool)
     */
    void set(bool state) {
        piston[0].set_value(state); // Sets the piston value to the value supplied
        piston_state = state;       // Updates the tracker
    }

    /**
     * Toggles the motor based on the tracker. State swaps to the opposite for the piston.
     */
    void toggle() {
        piston[0].set_value(!piston_state); // Sets the piston value to the opposite of the tracker value
        piston_state = !piston_state;       // Updates the tracker
    }

    /**
     * Gets the tracker value for the piston.
     * @return false (retracted) or true (extended)
     */
    bool get() {
        return piston_state; // Returns the piston_state tracker value
    }
};

/// A motor group wrapper that allows tracking and setting of motors in the array, initialize with MotorGroup.init(std::vector<int> motor_ports, bool alternating, bool initial_reverse_state);
class MotorGroup {
private:
    // int current_limiter;
    double current_speed;
    std::vector<pros::Motor> motors = {};
    double targetPosition; // For cross method execution tracking of target position
    bool movingToPosition;
    short reducedSpeed;
    int lastPosition;
    uint32_t lastTime;
    Robot::gearBox gearSet;

public:
    /** Initializes the motor group, defining all motors, configuring reverse states and hooking them up to an internal array
     * @param motor_ports | a vector of integers, containing the ports (in order) that you'd like to define (vector of ints w/ range [1 <-> 20])
     * @param alternating | should the motor reverse states alternate every other motor (bool w/ range [false <-> true])
     * @param initial_reverse_state | should the first motor be reversed (bool w/ range [false <-> true])
     * @return N/A
     */
    void init(std::vector<int> motor_ports, bool alternating,
              bool initial_reverse_state, Robot::gearBox gearSet) {
        if(alternating) {
            bool t =
                initial_reverse_state;                            // Reverse state is used, as the contructor
                                                                  // doesn't like us adjusting given variables
            for(int i = 0; i < motor_ports.size(); i++) {         // Goes through all the ports and sets them accordingly
                motors.push_back(pros::Motor(motor_ports[i], t)); // Initializes an array of motors,
                                                                  // given the ports and reversal states
                t = !t;                                           // Changes its state each time the loop occurs
            }
        } else { // If we are not supposed to alternate motor states, this runs
            for(int i = 0; i < motor_ports.size(); i++) {
                motors.push_back(pros::Motor(motor_ports[i], initial_reverse_state)); // Sets all motors to their ports and sets
                                                                                      // the motor reverse state to
                                                                                      // initial_reverse_state
            }
        }
        this->gearSet = gearSet;
        movingToPosition = false;
    }

    /** Sets the motor array to a specified speed
     * @param speed | a motor speed value (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void set(int speed) {
        current_speed = speed; // Sets the speed tracker
        for(int i = 0; i < motors.size(); i++) {
            motors[i] = speed;
        }
    }

    /** Starts a encoder movement by a specified number of degrees and at a certian speed
     * @param speed | a motor speed value (integer w/ range [-170 <-> 170])
     * @param byDegrees | a distance in degrees to move by (integer w/ range [-inf <-> inf])
     */
    void setPosition(int speed, int byDegrees) {
        tarePosition();
        targetPosition = byDegrees * (((double)gearSet) / 100) - (2500 / (double)gearSet);
        set(speed);
        movingToPosition = true;
        reducedSpeed = std::round(speed * .60);
        if(reducedSpeed < 15) {
            reducedSpeed = 15;
        }
    }

    /** Checks if the robot hit the position for the encoder movement
     * If it has, it will stop and set a few backend variables.
     */
    void checkPosition() {
        if(movingToPosition) {
            if(std::abs(getFastPosition()) > targetPosition) {
                set(0);
                targetPosition = 0;
                reducedSpeed = 0;
                movingToPosition = false;
                lastPosition = 0;
            } else if(std::abs(getFastPosition()) > targetPosition - targetPosition * .2 * (((double)(std::abs(getFastPosition() / (double)((uint32_t)pros::millis - lastTime) - lastPosition))))) {
                set(reducedSpeed);
            }
            lastPosition = std::abs(getFastPosition());
            lastTime = (uint32_t)pros::millis;
        }
    }

    /** Checks if the motor group is currently moving into an encoder specified position
     * @return true if moving, otherwise false
     */
    bool positionCheckStatus() {
        return movingToPosition;
    }

    /** Gets the current speed setting for the motor array
     * @return Current Motor Speed (integer w/ range [-170 <-> 170])
     */
    int getSpeed() { return current_speed; } // Fetches speed from tracker

    /** Tare the encoder position to the current position
     * @return N/A
     */
    void tarePosition() {
        for(int i = 0; i < motors.size(); i++) {
            motors[i].tare_position();
        }
    }

    /** Gets the average position for the array, use getFastPosition for only the first motor's position
     * @return average position for motor array (double w/ range [unknown, TODO add once known])
     */
    double getPosition() {
        double total = 0;
        for(int i = 0; i < motors.size(); i++) { // Loops for each motor, pulls velocities from each
            total += motors[i].get_position();
        }
        return total / motors.size(); // Returns the average of all positions
    }

    /** Gets the average velocity for the array, use getFastVelocity for only the first motor's position
     * @return average velocity for motor array (double w/ range [unknown, TODO add once known])
     */
    double getVelocity() {
        double total = 0;
        for(int i = 0; i < motors.size(); i++) { // Loops for each motor, pulls velocities from each
            total += motors[i].get_actual_velocity();
        }
        return total / motors.size();
    }

    /** Gets the velocity for the first motor in the array, use getVelocity for the average array position
     * @return velocity for first motor in the array (double w/ range [unknown, TODO add once known])
     */
    double getFastVelocity() {
        return motors[0].get_actual_velocity(); // Returns the velocity of the first motor
    }

    /** Gets the position for the first motor in the array, use getPosition for the average array position
     * @return position for first motor in the array (double w/ range [unknown, TODO add once known])
     */
    double getFastPosition() {
        return motors[0].get_position(); // Returns the position of the first motor
    }
    pros::Motor getDirectMotor(int motorIndex) {
        return motors[motorIndex];
    }
};

/// Wrapper for motor arrays, allows easy instanciation and passthrough to other classes
class Motor_Class {
public: // Phase out old Motor_Class functions (setSpeed, getSpeed, etc), and merely use as a holder for motor group objects - DONE
    /// Left Motor Group Object
    MotorGroup leftMotors;
    /// Right Motor Group Object
    MotorGroup rightMotors;
    /// Flywheel Motor Group Object
    // MotorGroup flywheelMotors;
    /// Spinner Motor Group Object
    MotorGroup spinnerMotors;
    /// Launcher Motor Group Object
    MotorGroup launcherMotors;
    /// Launcher Motor Group Object
    MotorGroup endGameMotors;
    /// Launcher Motor Group Object
    MotorGroup devMotors;
    /// Piston string launcher Object
    Piston stringLauncher;

public:
    Robot preset;

    /// Initializes the Motor Class wrapper with robot.cpp configuration, no external parameters required
    Motor_Class() {
        preset.init();
        leftMotors.init(preset.leftPorts, preset.leftAltRevStates[0], preset.leftAltRevStates[1], preset.leftGearbox);
        rightMotors.init(preset.rightPorts, preset.rightAltRevStates[0], preset.rightAltRevStates[1], preset.rightGearbox);
        spinnerMotors.init(preset.spinnerPorts, preset.spinnerAltRevStates[0], preset.spinnerAltRevStates[1], preset.spinnerGearbox);
        launcherMotors.init(preset.launcherPorts, preset.launcherAltRevStates[0], preset.launcherAltRevStates[1], preset.launcherGearbox);
        endGameMotors.init(preset.endGamePorts, preset.endGameAltRevStates[0], preset.endGameAltRevStates[1], preset.endGameGearbox);
        stringLauncher.init(preset.stringLauncherPort);

        if(preset.robotName == Robot::robotNames::Debug) {
            devMotors.init(preset.devMotorPorts, preset.devAltRevStates[0], preset.devAltRevStates[1], preset.devGearbox);
        }
    }

    void runPositionChecks() {
        leftMotors.checkPosition();
        rightMotors.checkPosition();
        spinnerMotors.checkPosition();
        launcherMotors.checkPosition();
    }

    bool self_test() {
        leftMotors.set(30);
        rightMotors.set(30);
        spinnerMotors.set(30);
        launcherMotors.set(30);
        pros::c::delay(300);
        bool result;
        for(int i = 0; i < 4; i++) {
            switch(i) {
            case 0:
                result = leftMotors.getFastVelocity() > 1 && leftMotors.getFastVelocity() < -1;
                break;

            case 1:
                result = rightMotors.getFastVelocity() > 1 && rightMotors.getFastVelocity() < -1 && result;
                break;

            case 2:
                result = spinnerMotors.getFastVelocity() > 1 && spinnerMotors.getFastVelocity() < -1 && result;
                break;

            case 3:
                result = launcherMotors.getFastVelocity() > 1 && launcherMotors.getFastVelocity() < -1 && result;
                break;
            }
        }
        leftMotors.set(0);
        rightMotors.set(0);
        spinnerMotors.set(0);
        launcherMotors.set(0);
        return result;
    }
};