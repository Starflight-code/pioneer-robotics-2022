// for driving motors
#include <string>
#include <vector>

/* DEPRECATED - Remove after testing updated preset system
const int num_motors_pre_side = 4,
          left_side_mtr_ports[num_motors_pre_side] = {1, 2, 3, 4},
          right_side_mtr_ports[num_motors_pre_side] = {5, 6, 7, 8};

// for flywheel

const int flywheel_mtr_count = 2,
          flywheel_mtr_ports[flywheel_mtr_count] = {9, 10};*/


/// Robot Presets
class robot {
    public:
    std::string name; // Robot Name
    int RID; // Robot Identification Number () 
    int controlScheme;
    double limiter;
    bool training = false; // Enables training/testing functionality that should be disabled at a competition
    std::vector<int> leftPorts; // Left Motor Port Array
    std::vector<int> rightPorts; // Right Motor Port Array
    std::vector<int> flywheelPorts; // Flywheel Motor Port Array
    std::vector<int> spinnerPorts; // Spinner Motor Port Array
    std::vector<bool> leftAlt_Rev_States;
    std::vector<bool> rightAlt_Rev_States;
    std::vector<bool> flywheelAlt_Rev_States;
    std::vector<bool> spinnerAlt_Rev_States;
    int launcher_port;

/// Should be 'a' for Artie or 'c' for Chance
    void init(char robot_initial)
    {
        char ri = robot_initial;
    RID = ((ri == 'a') * 1) + (ri == 'c') * 2; // Branchless method to generate robot identification numbers w/ characters
    switch(RID) {
        case 1: // Loads configs for Artie
        name = "Artie";
        leftPorts = {16, 6, 3, 8}; // Ports of left motors, from L1 to L4
        rightPorts = {20, 19, 18, 17}; // Ports of right motors, from R1 to R4
        flywheelPorts = {1, 4}; // Ports of flywheel motors, from F1 to F2
        spinnerPorts = {13}; // Port for the spinner motor
        leftAlt_Rev_States = {true,true};
        rightAlt_Rev_States = {true, false};
        flywheelAlt_Rev_States = {true, false};
        spinnerAlt_Rev_States = {false, false};
        controlScheme = 0; // 0 for tank, 1 for split arcade
        limiter = 1;
        launcher_port = 1; // Placeholder port given, replace once decided
        break;
        case 2: // Loads configs for Chance
        name = "Chance";
        leftPorts = {16, 6, 3, 8}; // Ports of left motors, from L1 to L4
        rightPorts = {20, 19, 18, 17}; // Ports of right motors, from R1 to R4
        flywheelPorts = {1, 4}; // Ports of flywheel motors, from F1 to F2
        spinnerPorts = {13}; // Port for the spinner motor
        leftAlt_Rev_States = {true,true};
        rightAlt_Rev_States = {true, false};
        flywheelAlt_Rev_States = {true, false};
        spinnerAlt_Rev_States = {false, false};
        controlScheme = 0; // 0 for tank, 1 for split arcade
        limiter = 1;
        launcher_port = 1; // Placeholder port given, replace once decided

        break;
    }
    
    }
};