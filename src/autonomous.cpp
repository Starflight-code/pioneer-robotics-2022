#ifndef absolute_positioning_cpp_
#define absolute_positioning_cpp_
#include "absolute_positioning.cpp"
#endif

#ifndef spinner_cpp_
#define spinner_cpp_
#include "spinner.cpp"
#endif

#ifndef autonomous_methods_cpp_
#include "autonomous_methods.cpp"
#define autonomous_methods_cpp_
#endif

#ifndef scheduler_cpp_
#include "scheduler.cpp"
#define scheduler_cpp_
#endif

#ifndef include_cpp_
#include "include.cpp"
#define include_cpp_
#endif

class Autonomous {
    cl* Control;
    AutonomousClass* Auton;
    Motor_Class* Motors;
    Robot* Preset;

public:
    Autonomous() {
        this->Control = new cl;
        this->Auton = new AutonomousClass;
        this->Motors = &Control->Motors;
        this->Preset = &Control->Motors.preset;
    }
    void run() {

        switch(Preset->robotName) {
        case Robot::Artie: // Artie
            artie();
            break;
        case Robot::Chance: // Chance
            chance();
            break;
        default:
            break;
        }
    }

    void artie() {
        Motors->leftMotors.set(20);
        Motors->rightMotors.set(20);
        Auton->forward(20, 160);
        Auton->spinner(80, 350);
    }

    void chance() {
        Motors->leftMotors.set(20);
        Motors->rightMotors.set(20);
        Auton->forward(20, 160);
        Auton->spinner(80, 350);
    }
};