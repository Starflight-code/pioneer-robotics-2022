#ifndef controls_cpp_
#include "controls.cpp"
#define controls_cpp_
#endif

class autonomous_class {
private:
    std::vector<uint32_t> cycleRun(short int taskIndex, std::vector<uint32_t> millis_array, short desiredWait) {
        // Integrated a historical task time skipping system, where if the wait time is greater than the historical
        // time taken for all other tasks on the cycle, processing is freed for other tasks. Leaving no impact on the task's timing.

        short lastTaskIndex = taskIndex - 1 < 0 ? millis_array.size() - 1 : taskIndex - 1;
        short waitTime = desiredWait - ((uint32_t)pros::millis() - millis_array[taskIndex]); // Calculates current wait time by checking desiredWaitTime(default:50) - time passed since task finished
        short historicWait = millis_array[lastTaskIndex] - millis_array[taskIndex];
        bool carryWaitTime = millis_array[taskIndex] % 100 == 0;
        if(not carryWaitTime) {                                   // Checks for a artifact from the historical wait time system
            millis_array[lastTaskIndex] = (uint32_t)pros::millis; // Prevents the index from reaching -1, instead setting it to the max index value if it is
        }                                                         // Sets current time as the finishing time of the last task
        if(waitTime > historicWait && not carryWaitTime) {
            millis_array[taskIndex] = millis_array[taskIndex] * 100;
        }
        pros::c::delay(waitTime < 0 || carryWaitTime ? waitTime / 100 > historicWait : waitTime > historicWait ? 0
                                                                                                               : waitTime); // Waits for the wait time, if it is still positive (has not passed)
        return millis_array;                                                                                                // Sends the updated array back, allows persistance between method executions
    }

private:
    std::vector<uint32_t> milis_cycle = {0, 0}; // Increase array size when adding new tasks (only up to current amount of tasks)
    short desiredWaitTime = 10;                 // per task wait in milliseconds
    bool task_one_finished;
    bool task_two_finished;

public:
    void forward(Motor_Class Motors, int distance, int speed) {
        task_one_finished = false;
        task_two_finished = false;
        Motors.leftMotors.tarePosition();
        Motors.rightMotors.tarePosition();
        Motors.leftMotors.set(speed);
        Motors.rightMotors.set(speed);

        while(not task_one_finished || not task_two_finished) {
            milis_cycle = cycleRun(0, milis_cycle, desiredWaitTime); // Should be placed before the task
            if(Motors.leftMotors.checkPosition(distance)) {
                Motors.leftMotors.set(0);
                task_one_finished = true;
            }
            milis_cycle = cycleRun(0, milis_cycle, desiredWaitTime); // Should be placed before the task
            if(Motors.rightMotors.checkPosition(distance)) {
                Motors.rightMotors.set(0);
                task_two_finished = true;
            }
        }
    }
    void turn(Motor_Class Motors, int distance, int speed, bool right) {
        task_one_finished = false;
        task_two_finished = false;
        Motors.leftMotors.tarePosition();
        Motors.rightMotors.tarePosition();
        if(!right) { // Turn Left
            Motors.leftMotors.set(-speed);
            Motors.rightMotors.set(speed);
        } else { // Turn Right
            Motors.leftMotors.set(speed);
            Motors.rightMotors.set(-speed);
        }

        while(not task_one_finished || not task_two_finished) {
            milis_cycle = cycleRun(0, milis_cycle, desiredWaitTime); // Should be placed before the task
            if(Motors.leftMotors.checkPosition(distance)) {
                Motors.leftMotors.set(0);
                task_one_finished = true;
            }
            milis_cycle = cycleRun(0, milis_cycle, desiredWaitTime); // Should be placed before the task
            if(Motors.rightMotors.checkPosition(distance)) {
                Motors.rightMotors.set(0);
                task_two_finished = true;
            }
        }
    }
};
