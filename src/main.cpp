#ifndef absolute_positioning_cpp_
#define absolute_positioning_cpp_
#include "absolute_positioning.cpp"
#include "pros/imu.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#endif

#ifndef spinner_cpp_
#define spinner_cpp_
#include "spinner.cpp"
#endif

#ifndef autonomous_cpp_
#include "autonomous.cpp"
#define autonomous_cpp_
#endif

#ifndef scheduler_cpp_
#include "scheduler.cpp"
#define scheduler_cpp_
#endif

#ifndef include_cpp_
#include "include.cpp"
#define include_cpp_
#endif

/** Contains the robot for isolation mode, runs a control loop that never exists.
 *  Consult a programming team member for further information before utilizing this.
 * @return N/A
 */
void isolation() {
    // Isolation Mode Restricted Execution Debug Enviroment

    // -- IMPORTS --
    cl control;
    AutonomousClass auton;
    // -- END OF IMPORTS --
    //  while(true) {
    //  auton.forward(control.Motors, 360, 30);
    control.Motors.devMotors.setPosition(40, 720);

    while(control.Motors.devMotors.positionCheckStatus()) {
        control.Motors.devMotors.checkPosition();
        pros::c::delay(10);
    }
    //}
    while(true) {
        pros::c::delay(50);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
}
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
}
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    double Move_Dist = 1;
    double Init_Pos;
    Motor_Class Motors;
    AutonomousClass auton;
    spin spinner;
    if(Motors.preset.isolation_mode) {
        isolation();
    }
    // pros::Rotation leftEncoders(Motors.Robot.rotationSensorPorts[0]);
    // pros::Rotation rightEncoders(Motors.Robot.rotationSensorPorts[1]);

    // Control_Algorithms pidOne(0.2, 0.05, 0.01);
    // Control_Algorithms pidTwo(0.2, 0.05, 0.01);
    // Motors.leftMotors.set(-1 * pidOne.PD_Velocity(200, abs(leftEncoders.get_velocity())));
    // Motors.rightMotors.set(-1 * pidTwo.PD_Velocity(200, abs(rightEncoders.get_velocity())));
    //    auton.forward(speed,distance)

    switch(Motors.preset.robotName) {
    case Robot::Artie: // Artie
        Motors.leftMotors.set(20);
        Motors.rightMotors.set(20);
        auton.forward(20, 160, Motors);
        auton.spinner(80, 350, Motors);
        break;
    case Robot::Chance: // Chance
        Motors.leftMotors.set(20);
        Motors.rightMotors.set(20);
        auton.forward(20, 160, Motors);
        auton.spinner(80, 350, Motors);
        break;
    default:
        break;
    }
}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

    u_short desiredWaitTime = 50; // per task wait in milliseconds
    cl Control_Listener;
    Scheduler tasks(2);
    if(Control_Listener.Motors.preset.isolation_mode) {
        isolation();
    }
    if(Control_Listener.Motors.preset.task_scheduler) {
        while(true) {

            tasks.cycleRun(0, desiredWaitTime); // Should be placed before the task
            Control_Listener.controls();        // Task 0

            tasks.cycleRun(1, desiredWaitTime);
            Control_Listener.event_listener(); // Task 1
        }
    } else {
        while(true) {
            Control_Listener.controls();
            Control_Listener.event_listener();
            pros::c::delay(50); // Waits 50 milliseconds and gives CPU some time to sleep. Increase this
                                // value if the CPU overheats.
        }
    }
}