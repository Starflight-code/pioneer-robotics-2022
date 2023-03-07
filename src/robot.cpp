#ifndef include_cpp_
#define include_cpp_
#include "include.cpp"
#endif

/// Robot Preset System allows centralized configuration of all configurable systems.
/// Configuration is hard coded and requires manual re-configuration to update it.
class robot {
public:
    int RID;          // Robot Identification Number [Internally Used]
    int DID;          // Driver Identification Number
    std::string name; // Robot Name [Artie or Chance]

    int controlScheme;                   // 0 for tank, 1 for split arcade
    double limiter;                      // Applies and tracks current limiter (range 0 <-> 1)
    bool debug = false;                  // Enables debug functionality, DISABLE BEFORE COMPETITION
    bool training = false;               // Enables training/testing functionality that should be disabled at a competition
    bool exponential_control = true;     // Enables exponent based control system
    double control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
    int control_switch_value;
    bool task_scheduler = true;
    std::vector<int> leftPorts;               // Left Motor Port Array
    std::vector<int> rightPorts;              // Right Motor Port Array
    std::vector<int> flywheelPorts;           // Flywheel Motor Port Array
    std::vector<int> spinnerPorts;            // Spinner Motor Port Array
    std::vector<bool> leftAlt_Rev_States;     // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<bool> rightAlt_Rev_States;    // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<bool> flywheelAlt_Rev_States; // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<bool> spinnerAlt_Rev_States;  // 0: Alternating (bool) 1: Initial Reverse State (bool)
    // int launcher_port;

    /** Initializes the Robot Preset System, all configuration is hard coded and this class does not accept any external parameters
     * @return N/A
     */
    void init() {
        // -- CONFIGURATIAON --
        char robot_initial = 'a';  // 'a' for Artie, 'c' for Chance or 'd' for debug
        char driver_initial = 'a'; // 'a' for Andrew, 'm' for Malachi, or 'd' for debug

        // Robot Identifier (Branchless robot initials to ID integer)
        RID = ((robot_initial == 'a') * 1) + ((robot_initial == 'c') * 2) + ((robot_initial == 'd') * 3);    // Branchless method to generate robot identification numbers w/ characters
        DID = ((driver_initial == 'a') * 1) + ((driver_initial == 'm') * 2 + ((driver_initial == 'd') * 3)); // Branchless method to generate driver identification numbers w/ characters
        switch(RID) {
        case 1: // Loads configuration for Artie
            name = "Artie";
            leftPorts = {9, 8, 5, 3};               // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 11};          // Ports of right motors, from R1 to R4
            flywheelPorts = {1, 4};                 // Ports of flywheel motors, from F1 to F2
            spinnerPorts = {13};                    // Port for the spinner motor
            leftAlt_Rev_States = {true, true};      // 0: Alternating (bool) 1: Initial Reverse State (bool)
            rightAlt_Rev_States = {true, false};    // Alternating: True, False, True ...
            flywheelAlt_Rev_States = {true, false}; // (Non Alternating) Initial Reverse State False: False, False, False
            spinnerAlt_Rev_States = {false, false}; // Initial Reverse State True: True, True, True
            controlScheme = 0;                      // 0 for tank, 1 for split arcade
            limiter = 1;
            // launcher_port = 1; // Removed, re-add for piston support
            break;

        case 2: // Loads configuration for Chance
            name = "Chance";
            leftPorts = {16, 6, 3, 8};         // Ports of left motors, from L1 to L4
            rightPorts = {20, 19, 18, 17};     // Ports of right motors, from R1 to R4
            flywheelPorts = {1, 4};            // Ports of flywheel motors, from F1 to F2
            spinnerPorts = {13};               // Port for the spinner motor
            leftAlt_Rev_States = {true, true}; // 0: Alternating (bool) 1: Initial Reverse State (bool)
            rightAlt_Rev_States = {true, false};
            flywheelAlt_Rev_States = {true, false};
            spinnerAlt_Rev_States = {false, false};
            controlScheme = 0; // 0 for tank, 1 for split arcade
            limiter = 1;
            // launcher_port = 1; // Removed, re-add for piston support
            break;

        case 3: // Loads debug configuration
            name = "Custom/Debug";
            leftPorts = {16, 6, 3, 8};     // Ports of left motors, from L1 to L4
            rightPorts = {20, 19, 18, 17}; // Ports of right motors, from R1 to R4
            flywheelPorts = {1, 4};        // Ports of flywheel motors, from F1 to F2
            spinnerPorts = {13};           // Port for the spinner motor
            leftAlt_Rev_States = {true, true};
            rightAlt_Rev_States = {true, false};
            flywheelAlt_Rev_States = {true, false};
            spinnerAlt_Rev_States = {false, false};
            controlScheme = 0; // 0 for tank, 1 for split arcade
            limiter = 1;
            // launcher_port = 1; // Removed, re-add for piston support
            debug = true; // Enables debug mode, can be used for verbose logging (not implimented yet)
            break;
        }
        switch(DID) {
        case 1:                           // Andrew - REPLACE ONCE NEW DRIVER IS DECIDED
            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;
            break;

        case 2:                           // Malachi
            controlScheme = 0;            // 0 for tank, 1 for split arcade
            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;
            break;

        case 3: // None

            break;
        }
    }
};