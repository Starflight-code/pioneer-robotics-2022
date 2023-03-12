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

class autonomous_class {
private:
    u_short desiredWaitTime = 10; // per task wait in milliseconds
    // bool task_one_finished;
    // bool task_two_finished;
    // bool task_finished;

public:
    void forward(Motor_Class Motors, int distance, int speed) {
        bool task_one_finished = false;
        bool task_two_finished = false;
        Motors.leftMotors.tarePosition();
        Motors.rightMotors.tarePosition();
        Motors.leftMotors.set(speed);
        Motors.rightMotors.set(speed);
        scheduler tasks(2);

        while(not task_one_finished || not task_two_finished) {
            tasks.cycleRun(0, desiredWaitTime);
            if(Motors.leftMotors.checkPosition(abs(distance))) {
                Motors.leftMotors.set(0);
                task_one_finished = true;
            }
            tasks.cycleRun(1, desiredWaitTime); // Should be placed before the task
            if(Motors.rightMotors.checkPosition(abs(distance))) {
                Motors.rightMotors.set(0);
                task_two_finished = true;
            }
        }
    }
    void spinner(Motor_Class Motors, int distance, int speed) {
        bool task_finished = false;
        Motors.spinnerMotors.tarePosition();
        Motors.spinnerMotors.set(speed);
        Motors.leftMotors.set(20);
        Motors.rightMotors.set(20);

        while(not task_finished) {
            if(Motors.spinnerMotors.checkPosition(abs(distance))) {
                Motors.spinnerMotors.set(0);
                task_finished = true;
            }
        }
        Motors.leftMotors.set(0);
        Motors.rightMotors.set(0);
    }
    void turn(Motor_Class Motors, int distance, int speed, bool right) {
        bool task_one_finished = false;
        bool task_two_finished = false;
        Motors.leftMotors.tarePosition();
        Motors.rightMotors.tarePosition();
        scheduler tasks(2);
        if(!right) { // Turn Left
            Motors.leftMotors.set(-speed);
            Motors.rightMotors.set(speed);
        } else { // Turn Right
            Motors.leftMotors.set(speed);
            Motors.rightMotors.set(-speed);
        }

        while(not task_one_finished || not task_two_finished) {
            tasks.cycleRun(0, desiredWaitTime); // Should be placed before the task
            if(Motors.leftMotors.checkPosition(distance)) {
                Motors.leftMotors.set(0);
                task_one_finished = true;
            }
            tasks.cycleRun(1, desiredWaitTime); // Should be placed before the task
            if(Motors.rightMotors.checkPosition(distance)) {
                Motors.rightMotors.set(0);
                task_two_finished = true;
            }
        }
    }
};
