#ifndef include_cpp_
#include "include.cpp"
#define include_cpp_
#endif

class scheduler {
    std::vector<uint32_t> millis_cycle; // length is equal to number of tasks

public:
    /** Initializes the scheduler system with the number of tasks to balance.
     * @param numberOfTasks you will be scheduling with the system (integer w/ range [1 <-> inf.])
     */
    scheduler(int numberOfTasks) {
        for(int i = 0; i < numberOfTasks; i++) {
            millis_cycle.push_back(0);
        }
    }

    /** Runs a task delay, making the task run desiredWait milliseconds after it finished
     * or bypassing it if the wait time is greater than the historical time taken for all
     * other tasks on the cycle, processing is freed for other tasks.
     * @param taskIndex task number starting at 0, counting up (integer w/ range [0 <-> inf.])
     * @param desiredWait milliseconds for this task to run after last execution (unsigned short integer w/ range (0 <-> 65,535))
     */
    void cycleRun(short int taskIndex, u_short desiredWait) {
        // Integrated a historical task time skipping system, where if the wait time is greater than the historical
        // time taken for all other tasks on the cycle, processing is freed for other tasks. Leaving no impact on the task's timing.

        short lastTaskIndex = taskIndex - 1 < 0 ? millis_cycle.size() - 1 : taskIndex - 1;
        short waitTime = desiredWait - ((uint32_t)pros::millis() - millis_cycle[taskIndex]); // Calculates current wait time by checking desiredWaitTime(default:50) - time passed since task finished
        short historicWait = millis_cycle[lastTaskIndex] - millis_cycle[taskIndex];
        bool carryWaitTime = millis_cycle[taskIndex] % 100 == 0;
        if(not carryWaitTime) {                                   // Checks for a artifact added by the historical wait time system
            millis_cycle[lastTaskIndex] = (uint32_t)pros::millis; // Prevents the index from reaching -1, instead setting it to the max index value if it is
        }                                                         // Sets current time as the finishing time of the last task
        if(waitTime > historicWait && not carryWaitTime) {
            millis_cycle[taskIndex] = millis_cycle[taskIndex] * 100;
        }
        pros::c::delay(waitTime < 0 || carryWaitTime ? waitTime / 100 > historicWait : waitTime > historicWait ? 0
                                                                                                               : waitTime); // Waits for the wait time, if it is still positive (has not passed)                                                                                           // Sends the updated array back, allows persistance between method executions
    }
};