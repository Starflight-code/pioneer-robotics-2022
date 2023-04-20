#include "pros/adi.hpp"
#include "pros/rtos.h"
#ifndef main_h_
#include "main.h"
#define main_h_
#endif

#ifndef robot_cpp_
#include "robot.cpp"
#define robot_cpp_
#endif

#ifndef encoder_interface_cpp_
// #include "encoder_interface.cpp"
#define encoder_interface_cpp_
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
    int targetPosition; // For cross method execution tracking of target position
    robot::gearBox gear;
    bool movingToPosition;

public:
    /** Initializes the motor group, defining all motors, configuring reverse states and hooking them up to an internal array
     * @param motor_ports | a vector of integers, containing the ports (in order) that you'd like to define (vector of ints w/ range [1 <-> 20])
     * @param alternating | should the motor reverse states alternate every other motor (bool w/ range [false <-> true])
     * @param initial_reverse_state | should the first motor be reversed (bool w/ range [false <-> true])
     * @return N/A
     */
    void init(std::vector<int> motor_ports, bool alternating,
              bool initial_reverse_state, robot::gearBox gearBox) {
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
            gear = gearBox;
            movingToPosition = false;
        }
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
    void setPosition(int speed, int byDegrees) {
        tarePosition();
        targetPosition = byDegrees * ((double)gear / 100);
        set(speed);
        movingToPosition = true;
    }
    void checkPosition() {
        if(movingToPosition) {
            if(std::abs(getFastPosition()) > targetPosition) {
                set(0);
                targetPosition = NULL;
                movingToPosition = false;
            }
        }
    }

    /// Returns true if moving to position, otherwise false
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
    /*bool checkPosition(int targetPosition) {
        if(std::abs(motors[0].get_position()) >= targetPosition) {
            return true;
        }
        return false;
    }*/

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
    /// Piston string launcher Object
    Piston stringLauncher;

public:
    robot Robot;

    /// Initializes the Motor Class wrapper with robot.cpp configuration, no external parameters required
    Motor_Class() {
        Robot.init();
        leftMotors.init(Robot.leftPorts, Robot.leftAltRevStates[0], Robot.leftAltRevStates[1], Robot.leftGearbox);
        rightMotors.init(Robot.rightPorts, Robot.rightAltRevStates[0], Robot.rightAltRevStates[1], Robot.rightGearbox);
        spinnerMotors.init(Robot.spinnerPorts, Robot.spinnerAltRevStates[0], Robot.spinnerAltRevStates[1], Robot.spinnerGearbox);
        launcherMotors.init(Robot.launcherPorts, Robot.launcherAltRevStates[0], Robot.launcherAltRevStates[1], Robot.launcherGearbox);
        stringLauncher.init(Robot.stringLauncherPort);
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