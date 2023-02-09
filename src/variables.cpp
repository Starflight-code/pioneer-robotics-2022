// for driving motors
#include "pros/llemu.hpp"
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
    int RID; // Robot Identification Number
    int DID; // Driver Identification Number
    int controlScheme;
    double limiter;
    bool debug = false; // Enables debug functionality, DISABLE BEFORE COMPETITION
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

/// Should be 'a' for Artie, 'c' for Chance or 'd' for debug (custom/nonspecific robot)
/// Should be 'b' for Bryce, 'm' for Malachi, or 'd' for debug (no override)
    void init(char robot_initial, char driver_initial)
    {
    // -- OVERRIDE --
    //robot_initial = 'a';
    //driver_initial = 'm';

    // Robot Identifier (Branchless robot initials to ID integer)
    RID = ((robot_initial == 'a') * 1) + ((robot_initial == 'c') * 2) + ((robot_initial == 'd') * 3); // Branchless method to generate robot identification numbers w/ characters
    DID = ((driver_initial == 'b') * 1) + ((driver_initial == 'm') * 2 + ((driver_initial == 'd') * 3)); // Branchless method to generate driver identification numbers w/ characters
    //RID = 1 or 2 Override whatever was given
    switch(RID) {
        case 1: // Loads configs for Artie
        //pros::lcd::print(1, "Artie");
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
        
        case 3: // Loads configs for Debug
        name = "Custom/Debug";
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
        debug = true; // Enables debug mode, can be used for verbose logging (not implimented yet)
        break;
    }
    switch(DID) {
        case 1: // Bryce

        break;

        case 2: // Malachi
        controlScheme = 0; // 0 for tank, 1 for split arcade
        break;
        
        case 3: // None

        break;
    }
    
    
    }
};