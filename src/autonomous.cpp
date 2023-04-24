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
    // bool task_one_finished;
    // bool task_two_finished;
    // bool task_finished;

public:
    /** Moves forward by a distance at a speed
     * @param Motors | the motor wrapper (class object)
     * @param distance | a distance in degrees to move by (integer w/ range [-inf <-> inf])
     * @param speed | a speed to set the motors to (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void forward(Motor_Class Motors, int distance, int speed) {
        Motors.leftMotors.setPosition(speed, distance);
        Motors.rightMotors.setPosition(speed, distance);

        while(Motors.leftMotors.positionCheckStatus() || Motors.rightMotors.positionCheckStatus()) {
            Motors.leftMotors.checkPosition();
            Motors.rightMotors.checkPosition();
            pros::c::delay(20);
        }
    }

    /** Moves the spinners by a specified distance at a specified speed
     * Applies a hard coded forward boost to increase contact with spinners
     * @param Motors | the motor wrapper (class object)
     * @param distance | a distance in degrees to move by (integer w/ range [-inf <-> inf])
     * @param speed | a speed to set the motors to (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void spinner(Motor_Class Motors, int distance, int speed) {
        Motors.spinnerMotors.setPosition(speed, distance);
        Motors.leftMotors.set(20);
        Motors.rightMotors.set(20);

        while(Motors.spinnerMotors.positionCheckStatus()) {
            Motors.spinnerMotors.checkPosition();
            pros::c::delay(50);
        }
        Motors.leftMotors.set(0);
        Motors.rightMotors.set(0);
    }

    /** Turns the robot by a specified distance and at a specified speed
     * @param Motors | the motor wrapper (class object)
     * @param distance | a distance in degrees to move each motor group by (integer w/ range [-inf <-> inf])
     * @param speed | a speed to set the motors to (integer w/ range [-170 <-> 170])
     * @return N/A
     */
    void turn(Motor_Class Motors, int distance, int speed, bool right) {
        if(!right) { // Turn Left
            Motors.leftMotors.setPosition(-speed, 90);
            Motors.rightMotors.setPosition(speed, 90);
        } else { // Turn Right
            Motors.leftMotors.setPosition(speed, 90);
            Motors.rightMotors.setPosition(-speed, 90);
        }

        while(Motors.leftMotors.positionCheckStatus() || Motors.rightMotors.positionCheckStatus()) {
        }
    }
};
