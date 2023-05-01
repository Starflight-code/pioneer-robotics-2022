#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#ifndef controls_cpp_
#include "controls.cpp"
#define controls_cpp_
#endif

#ifndef scheduler_cpp_
#define scheduler_cpp_
#include "scheduler.cpp"
#endif

#ifndef spinner_cpp_
#include "spinner.cpp"
#define spinner_cpp_
#endif

class AutonomousClass {
private:
    u_short desiredWaitTime = 10; // per task wait in milliseconds
    // Motor_Class motors;
    //  bool task_one_finished;
    //  bool task_two_finished;
    //  bool task_finished;
    Motor_Class* motors;

public:
    AutonomousClass(Motor_Class motorIn) {
        this->motors = &motorIn;
    }
    /** Moves forward by a distance at a speed
     * @param Motors | the motor wrapper (class object)
     * @param distance | a distance in degrees to move by (integer w/ range [-inf <-> inf])
     * @param speed | a speed to set the motors to (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void forward(int speed, int distance, Motor_Class motors) {
        motors.leftMotors.setPosition(speed, distance);
        motors.rightMotors.setPosition(speed, distance);

        while(motors.leftMotors.positionCheckStatus() || motors.rightMotors.positionCheckStatus()) {
            motors.leftMotors.checkPosition();
            motors.rightMotors.checkPosition();
            pros::c::delay(20);
        }
    }

    /** Start motors moving forward at a specified speed
     * @param speed | speed to set motors to (integer w/ range [-127 <-> 127])
     * @return N/A
     */
    void forward(int speed, Motor_Class motors) {
        motors.leftMotors.set(speed);
        motors.rightMotors.set(speed);
    }

    /** Starts motors moving forward at a speed for an amount of time
     * @param speed | speed to set motors to (integer w/ range [-127 <-> 127])
     * @param milliseconds | time to leave motors at specified speed (integer w/ range [0 <-> inf])
     * @return N/A
     */
    void forwardFor(int speed, int milliseconds /*, Motor_Class motors*/) {
        motors->leftMotors.set(speed);
        motors->rightMotors.set(speed);
        pros::c::delay(milliseconds);
        motors->leftMotors.set(0);
        motors->rightMotors.set(0);
    }

    /** Stops the motors for the drive motor groups
     * @return N/A
     */
    void forwardStop(Motor_Class motors) {
        motors.leftMotors.set(0);
        motors.rightMotors.set(0);
    }

    /** Moves the spinners by a specified distance at a specified speed
     * Applies a hard coded forward boost to increase contact with spinners
     * @param Motors | the motor wrapper (class object)
     * @param distance | a distance in degrees to move by (integer w/ range [-inf <-> inf])
     * @param speed | a speed to set the motors to (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void spinner(int distance, int speed, Motor_Class motors) {
        motors.spinnerMotors.setPosition(speed, distance);
        motors.leftMotors.set(20);
        motors.rightMotors.set(20);

        while(motors.spinnerMotors.positionCheckStatus()) {
            motors.spinnerMotors.checkPosition();
            pros::c::delay(50);
        }
        motors.leftMotors.set(0);
        motors.rightMotors.set(0);
    }

    /** Turns the robot by a specified distance and at a specified speed
     * @param Motors | the motor wrapper (class object)
     * @param distance | a distance in degrees to move each motor group by (integer w/ range [-inf <-> inf])
     * @param speed | a speed to set the motors to (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void turn(int distance, int speed, bool right, Motor_Class motors) {
        if(!right) { // Turn Left
            motors.leftMotors.setPosition(-speed, 90);
            motors.rightMotors.setPosition(speed, 90);
        } else { // Turn Right
            motors.leftMotors.setPosition(speed, 90);
            motors.rightMotors.setPosition(-speed, 90);
        }

        while(motors.leftMotors.positionCheckStatus() || motors.rightMotors.positionCheckStatus()) {
        }
    }

    /** Toggles the string launcher, dropping it if it's extended
     * @return N/A
     */
    void dropPiston(Motor_Class motors) {
        motors.stringLauncher.toggle();
    }

    /** Sets the intake to a specified speed
     * @param speed | speed to set the intake motors to (integer w/ range [-127 <-> 127])
     * @return N/A
     */
    void runIntake(int speed, Motor_Class motors) {
        motors.spinnerMotors.set(speed);
    }

    /** Runs the intake at a specified speed for a specified amount of time
     * @param speed | speed to set the intake motors to (integer w/ range [-127 <-> 127])
     * @param milliseconds | time to leave the motors active (integer w/ range [0 <-> inf])
     * @return N/A
     */
    void runIntake(int speed, int milliseconds, Motor_Class motors) {
        motors.spinnerMotors.set(speed);
        pros::c::delay(milliseconds);
        motors.spinnerMotors.set(0);
    }

    /** Pulls back the launcher at a specified speed by a specified distance
     * @param speed | speed to set the launcher to
     * @param degrees | distance to pull the launcher back
     * @return N/A
     */
    void pullbackLauncher(int speed, int degrees, Motor_Class motors) {
        motors.launcherMotors.setPosition(speed, degrees);
        while(motors.launcherMotors.positionCheckStatus()) {
            motors.launcherMotors.checkPosition();
            pros::c::delay(20);
        }
    }

    /** Sets the launcher to a specified speed
     * @param speed | speed to set the launcher to
     * @return N/A
     */
    void pullbackLauncher(int speed, Motor_Class motors) {
        motors.launcherMotors.set(speed);
    }

    /** Pulls back the launcher at a specified speed for a specified amount of time
     * @param speed | speed to set launcher motors to
     * @param milliseconds | time to leave launcher motors active
     * @return N/A
     */
    void pullbackLauncherFor(int speed, int milliseconds, Motor_Class motors) {
        motors.launcherMotors.set(speed);
        pros::c::delay(milliseconds);
        motors.launcherMotors.set(0);
    }

    /** Waits for a number of milliseconds
     * @param milliseconds | time to wait for
     * @return N/A
     */
    void wait(int milliseconds) {
        pros::c::delay(milliseconds);
    }
};
