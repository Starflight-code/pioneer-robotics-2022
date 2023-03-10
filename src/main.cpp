#ifndef absolute_positioning_cpp_
#define absolute_positioning_cpp_
#include "absolute_positioning.cpp"
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
    autonomous_class auton;
    auton.forward(Motors, 1200, 30);
    // auton.turn(Motors, 650, 20, true); // Turns 180 degrees, do not change distance value (for now)
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
/*std::vector<uint32_t> cycleRun(short int taskIndex, std::vector<uint32_t> millis_array, short desiredWait) {
    // Integrated a historical task time skipping system, where if the wait time is greater than the historical
    // time taken for all other tasks on the cycle, processing is freed for other tasks. Leaving no impact on the task's timing.

    short lastTaskIndex = taskIndex - 1 < 0 ? millis_array.size() - 1 : taskIndex - 1;
    short waitTime = desiredWait - ((uint32_t)pros::millis() - millis_array[taskIndex]); // Calculates current wait time by checking desiredWaitTime(default:50) - time passed since task finished
    short historicWait = millis_array[lastTaskIndex] - millis_array[taskIndex];
    bool carryWaitTime = millis_array[taskIndex] % 100 == 0;
    if(not carryWaitTime) {                                   // Checks for a artifact added by the historical wait time system
        millis_array[lastTaskIndex] = (uint32_t)pros::millis; // Prevents the index from reaching -1, instead setting it to the max index value if it is
    }                                                         // Sets current time as the finishing time of the last task
    if(waitTime > historicWait && not carryWaitTime) {
        millis_array[taskIndex] = millis_array[taskIndex] * 100;
    }
    pros::c::delay(waitTime < 0 || carryWaitTime ? waitTime / 100 > historicWait : waitTime > historicWait ? 0
                                                                                                           : waitTime); // Waits for the wait time, if it is still positive (has not passed)
    return millis_array;                                                                                                // Sends the updated array back, allows persistance between method executions
}*/
void opcontrol() {
    u_short desiredWaitTime = 50; // per task wait in milliseconds
    cl Control_Listener;
    scheduler tasks(2);
    if(Control_Listener.Motors.Robot.task_scheduler) {
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
