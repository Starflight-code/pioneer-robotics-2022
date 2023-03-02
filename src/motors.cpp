#include "pros/motors.hpp"
#include "include.cpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"
#include "robot.cpp"
#include <algorithm>
#include <array>
#include <vector>

// CONTAINS A LOT OF DEPRECATED CODE - Will be removed once it's tested

/*class Piston { // Piston class, supports ADI Pistons and includes tracking/setting
DO NOT REMOVE, until build team has confirmed we are not using pistons on the robot (even then, build team may end up adding them later...)
private:
    std::vector<pros::ADIDigitalOut> piston = {};
    bool piston_state; // Allows for toggle/state fetching
public:
    void init(int ADI_Port) { // Initializes the piston, this needs to be completed before it can be used
        piston.push_back(pros::ADIDigitalOut(ADI_Port));
        piston[0].set_value(false); // Retracts the piston, syncing it's state
        piston_state = false;       // Sets the piston state, allowing for tracking
    }
    void set(bool state) {
        piston[0].set_value(state); // Sets the piston value to whatever was supplied
        piston_state = state;       // Updates the tracker
    }
    bool get() {
        return piston_state; // Returns the piston_state tracker value
    }
};*/

/// A motor group wrapper that allows tracking and setting of motors in the array, initialize with MotorGroup.init(std::vector<int> motor_ports, bool alternating, bool initial_reverse_state);
class MotorGroup {
private:
    // int current_limiter;
    double current_speed;
    std::vector<pros::Motor> motors = {};

public:
    /** Initializes the motor group, defining all motors, configuring reverse states and hooking them up to an internal array
     * @param motor_ports | a vector of integers, containing the ports (in order) that you'd like to define (vector of ints w/ range [1 <-> 20])
     * @param alternating | should the motor reverse states alternate every other motor (bool w/ range [false <-> true])
     * @param initial_reverse_state | should the first motor be reversed (bool w/ range [false <-> true])
     * @return N/A
     */
    void init(std::vector<int> motor_ports, bool alternating,
              bool initial_reverse_state) {
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

    /** Gets the current speed setting for the motor array
     * @return Current Motor Speed (integer w/ range [-170 <-> 170])
     */
    int getSpeed() { return current_speed; } // Fetches speed from tracker

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
};
/// Wrapper for motor arrays, allows easy instanciation and passthrough to other classes
class Motor_Class {
public: // Phase out old Motor_Class functions (setSpeed, getSpeed, etc), and merely use as a holder for motor group objects - DONE
    /// Left Motor Group Object
    MotorGroup leftMotors;
    /// Right Motor Group Object
    MotorGroup rightMotors;
    /// Flywheel Motor Group Object
    MotorGroup flywheelMotors;
    /// Spinner Motor Group Object
    MotorGroup spinnerMotors;
    /// Array containing all motor group objects for unique scripting
    std::array<MotorGroup, 4> Motors = {leftMotors, rightMotors, flywheelMotors, spinnerMotors};
    // Piston launcher;

public:
    robot Robot;

    /// Initializes the Motor Class wrapper with robot.cpp configuration, no external parameters required
    Motor_Class() {
        Robot.init(/*'a', 'm'*/); // Sets up preset for robot 'a' for Artie and 'c' for Chance
        leftMotors.init(Robot.leftPorts, Robot.leftAlt_Rev_States[0], Robot.leftAlt_Rev_States[1]);
        rightMotors.init(Robot.rightPorts, Robot.rightAlt_Rev_States[0], Robot.rightAlt_Rev_States[1]);
        flywheelMotors.init(Robot.flywheelPorts, Robot.flywheelAlt_Rev_States[0], Robot.flywheelAlt_Rev_States[1]);
        spinnerMotors.init(Robot.spinnerPorts, Robot.spinnerAlt_Rev_States[0], Robot.spinnerAlt_Rev_States[1]);
    }
};