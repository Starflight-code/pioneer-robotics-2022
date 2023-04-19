#ifndef include_cpp_
#define include_cpp_
#include "include.cpp"
#include "pros/misc.h"
#include <vector>
#endif

/// Robot Preset System allows centralized configuration of all configurable systems.
/// Configuration is hard coded and requires manual re-configuration to update it.
class robot {
public:
    // int RID; // Robot Identification Number [Used Internally]
    // int DID; // Driver Identification Number

    // Enumerations, used interally and for configuration
    enum robotNames { Artie,
                      Chance,
                      Debug };
    enum driverNames { Teagen,
                       Malachi,
                       None };
    enum controlSchemes { Tank,
                          Arcade };
    robotNames robotName;
    driverNames driverName;
    controlSchemes controlScheme;

    // Variable - Globally Configurable
    int spinner_speed = 50;
    int spinner_boost = 20;

    // Variables - Not Globally Configurable
    double limiter;                      // Applies and tracks current limiter (range 0 <-> 1)
    bool debug = false;                  // Enables debug functionality, DISABLE BEFORE COMPETITION
    bool training = false;               // Enables training/testing functionality that should be disabled at a competition
    bool exponential_control = true;     // Enables exponent based control system
    double control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
    int control_switch_value;
    bool task_scheduler = true;
    double left_right_motor_offset; // Negative values are a left offset, positive is a right
    // Value what the motors will be multiplied by (should be range [-1 <-> 1]), non-dynamic drive adjustment
    std::vector<int> leftPorts;                               // Left Motor Port Array
    std::vector<int> rightPorts;                              // Right Motor Port Array
    std::vector<int> flywheelPorts;                           // Flywheel Motor Port Array
    std::vector<int> spinnerPorts;                            // Spinner Motor Port Array
    std::vector<bool> leftAltRevStates;                       // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<bool> rightAltRevStates;                      // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<bool> flywheelAltRevStates;                   // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<bool> spinnerAltRevStates;                    // 0: Alternating (bool) 1: Initial Reverse State (bool)
    std::vector<int> rotationSensorPorts;                     // Left, Right motor array encoders
    std::vector<int> launcherPorts;                           // Left, Right launcher pullback array
    std::vector<bool> launcherAltRevStates;                   // 0: Alternating (bool) 1: Initial Reverse State (bool)
    int stringLauncherPort;                                   // Port for endgame string launcher (ADI, piston)
    std::vector<pros::controller_digital_e_t> controlButtons; // Array containing customizable control buttons

    /** Initializes the Robot Preset System, all configuration is hard coded and this class does not accept any external parameters
     * @return N/A
     */
    void init() {
        // -- CONFIGURATIAON --
        // char robot_initial = 'a';  // 'a' for Artie, 'c' for Chance or 'd' for debug
        // char driver_initial = 'a'; // 't' for Teagan, 'm' for Malachi, or 'd' for debug
        robotName = Artie;
        driverName = Malachi;
        // Robot Identifier (Branchless robot initials to ID integer)
        // RID = ((name == Artie) * 1) + ((name == Chance) * 2) + ((name == Debug) * 3);     // Branchless method to generate robot identification numbers w/ characters
        // DID = ((dName == Teagen) * 1) + ((dName == Malachi) * 2 + ((dName == None) * 3)); // Branchless method to generate driver identification numbers w/ characters
        switch(robotName) {
        case Artie: // Loads configuration for Artie

            // Motor Ports
            leftPorts = {9, 8, 5, 3};      // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 12}; // Ports of right motors, from R1 to R4
            flywheelPorts = {1, 4};        // Ports of flywheel motors, from F1 to F2
            spinnerPorts = {6, 16};        // Port for the spinner motor
            launcherPorts = {1, 2};        // Ports for the disk launcher
            rotationSensorPorts = {2, 17}; // Ports for the rotation sensors/encoders
            stringLauncherPort = 1;        // Port for the string launcher piston

            // Motor Tuning
            leftAltRevStates = {true, true};      // 0: Alternating (bool) 1: Initial Reverse State (bool)
            rightAltRevStates = {true, false};    // Alternating: True, False, True ...
            flywheelAltRevStates = {true, false}; // (Non Alternating) Initial Reverse State False: False, False, False
            spinnerAltRevStates = {true, false};  // Initial Reverse State True: True, True, True
            launcherAltRevStates = {true, false};

            // Control Tuning
            controlScheme = Tank; // 0 for tank, 1 for split arcade
            left_right_motor_offset = 0;
            limiter = 1;

            break;

        case Chance: // Loads configuration for Chance

            // Motor Ports
            leftPorts = {9, 8, 5, 3};      // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 12}; // Ports of right motors, from R1 to R4
            flywheelPorts = {1, 4};        // Ports of flywheel motors, from F1 to F2
            spinnerPorts = {6, 16};        // Port for the spinner motor
            launcherPorts = {1, 2};        // Ports for the disk launcher
            rotationSensorPorts = {2, 17}; // Ports for the rotation sensors/encoders
            stringLauncherPort = 1;        // Port for the string launcher piston

            // Motor Tuning
            leftAltRevStates = {true, true};      // 0: Alternating (bool) 1: Initial Reverse State (bool)
            rightAltRevStates = {true, false};    // Alternating: True, False, True ...
            flywheelAltRevStates = {true, false}; // (Non Alternating) Initial Reverse State False: False, False, False
            spinnerAltRevStates = {true, false};  // Initial Reverse State True: True, True, True
            launcherAltRevStates = {true, false};

            // Control Tuning
            controlScheme = Tank; // 0 for tank, 1 for split arcade
            left_right_motor_offset = 0;
            limiter = 1;

            break;

        case Debug: // Loads debug configuration

            // Motor Ports
            leftPorts = {9, 8, 5, 3};      // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 12}; // Ports of right motors, from R1 to R4
            flywheelPorts = {1, 4};        // Ports of flywheel motors, from F1 to F2
            spinnerPorts = {6, 16};        // Port for the spinner motor
            launcherPorts = {1, 2};        // Ports for the disk launcher
            rotationSensorPorts = {2, 17}; // Ports for the rotation sensors/encoders
            stringLauncherPort = 1;        // Port for the string launcher piston

            // Motor Tuning
            leftAltRevStates = {true, true};      // 0: Alternating (bool) 1: Initial Reverse State (bool)
            rightAltRevStates = {true, false};    // Alternating: True, False, True ...
            flywheelAltRevStates = {true, false}; // (Non Alternating) Initial Reverse State False: False, False, False
            spinnerAltRevStates = {true, false};  // Initial Reverse State True: True, True, True
            launcherAltRevStates = {true, false};

            // Control Tuning
            controlScheme = Tank; // 0 for tank, 1 for split arcade
            left_right_motor_offset = 0;
            limiter = 1;

            break;
        }
        switch(driverName) {
        case Teagen:

            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;              // Sets the training mode, FALSE FOR COMPETITIONS

            break;

        case Malachi:

            controlScheme = Tank;         // 0 for tank, 1 for split arcade
            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;              // Sets the training mode, FALSE FOR COMPETITIONS

            break;

        case None:

            controlScheme = Tank;         // 0 for tank, 1 for split arcade
            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;              // Sets the training mode, FALSE FOR COMPETITIONS

            break;
        }
        control_scheme_setup();
    }
    /**
     * Sets up the control scheme based on driver presets
     */
    void control_scheme_setup() {
        switch(driverName) {
        case Teagen:
            // [Training Local Limiter Button, Piston Button, Spinner Normal Button, Spinner Reversed Button, Control Reverse Toggle]

            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_LEFT); // Training Local Limiter Button
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_Y);    // Piston Keybind
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_R1);   // Spinner Keybind (Normal)
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_R2);   // Spinner Keybind (Reversed)
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_A);    // Control Reverse Toggle

            break;
        case Malachi:

            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_LEFT); // Training Local Limiter Button
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_Y);    // Piston Keybind
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_R1);   // Spinner Keybind (Normal)
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_R2);   // Spinner Keybind (Reversed)
            controlButtons.push_back(pros::E_CONTROLLER_DIGITAL_A);    // Control Reverse Toggle

            break;
        default:
            break;
        }
    }
};