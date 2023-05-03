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

class isolation_mode {
    cl* Control;
    AutonomousClass* Auton;
    Motor_Class* Motors;
    Robot* Preset;

public:
    isolation_mode() {
        this->Control = new cl;
        this->Auton = new AutonomousClass;
        this->Motors = &Control->Motors;
        this->Preset = &Control->Motors.preset;
    }
    void run() {
    }
};