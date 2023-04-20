#ifndef include_cpp_
#define include_cpp_
#include "include.cpp"
#include "pros/misc.h"
#include <vector>
#endif

// (C) indicates a configurable option, (S) indicates a static, non configurable option.

/// Robot Preset System allows centralized configuration of all configurable systems.
/// Configuration is hard coded and requires manual re-configuration to update it.
class robot {
public:
    // Enumerations, used interally and for configuration (S)
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

    // Variable - Globally Configurable (C)
    int spinner_speed = 50;
    int spinner_boost = 20;
    bool debug = false;          // Enables debug functionality, DISABLE BEFORE COMPETITION
    bool training = false;       // Enables training/testing functionality that should be disabled at a competition
    bool isolation_mode = false; // Isolated debug mode for highly experimental systems, DO NOT TURN THIS ON UNLESS YOU KNOW WHAT YOU'RE DOING

    // Variables - Not Globally Configurable (S)
    double limiter;                      // Applies and tracks current limiter (range 0 <-> 1)
    bool exponential_control = true;     // Enables exponent based control system
    double control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
    int control_switch_value;
    bool task_scheduler = true;
    double left_right_motor_offset; // Negative values are a left offset, positive is a right
    // Value what the motors will be multiplied by (should be range [-1 <-> 1]), non-dynamic drive adjustment
    std::vector<int> leftPorts;                               // Left Motor Port Array
    std::vector<int> rightPorts;                              // Right Motor Port Array
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
        // -- CONFIGURATIAON -- (C)
        robotName = Artie;
        driverName = Malachi;
        switch(robotName) {

        case Artie: // Loads configuration for Artie (C)

            // Motor Ports
            leftPorts = {9, 8, 5, 3};      // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 12}; // Ports of right motors, from R1 to R4
            spinnerPorts = {6, 16};        // Port for the spinner motor from S1 to S2
            launcherPorts = {1, 2};        // Ports for the disk launcher L1 to L2
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

        case Chance: // Loads configuration for Chance (C)

            // Motor Ports
            leftPorts = {9, 8, 5, 3};      // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 12}; // Ports of right motors, from R1 to R4
            spinnerPorts = {6, 16};        // Port for the spinner motor from S1 to S2
            launcherPorts = {1, 2};        // Ports for the disk launcher L1 to L2
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

        case Debug: // Loads debug configuration (C)

            // Motor Ports
            leftPorts = {9, 8, 5, 3};      // Ports of left motors, from L1 to L4
            rightPorts = {20, 18, 14, 12}; // Ports of right motors, from R1 to R4
            spinnerPorts = {6, 16};        // Port for the spinner motor from S1 to S2
            launcherPorts = {1, 2};        // Ports for the disk launcher L1 to L2
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
        case Teagen: // (C)

            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;              // Sets the training mode, FALSE FOR COMPETITIONS

            break;

        case Malachi: // (C)

            controlScheme = Tank;         // 0 for tank, 1 for split arcade
            exponential_control = true;   // Enables exponent based control system
            control_exponent_value = 1.5; // Greater the value, the steeper the exponential control curve
            training = true;              // Sets the training mode, FALSE FOR COMPETITIONS

            break;

        case None: // (C)

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

        case Teagen: // (C)

            // [Training Local Limiter Button, Piston Button, Spinner Normal Button, Spinner Reversed Button, Control Reverse Toggle]
            controlButtons = {
                pros::E_CONTROLLER_DIGITAL_LEFT, // Training Local Limiter Button
                pros::E_CONTROLLER_DIGITAL_Y,    // Piston Keybind
                pros::E_CONTROLLER_DIGITAL_R1,   // Spinner Keybind (Normal)
                pros::E_CONTROLLER_DIGITAL_R2,   // Spinner Keybind (Reversed)
                pros::E_CONTROLLER_DIGITAL_A     // Control Reverse Toggle
            };

            break;
        case Malachi: // (C)

            controlButtons = {
                pros::E_CONTROLLER_DIGITAL_LEFT, // Training Local Limiter Button
                pros::E_CONTROLLER_DIGITAL_Y,    // Piston Keybind
                pros::E_CONTROLLER_DIGITAL_R1,   // Spinner Keybind (Normal)
                pros::E_CONTROLLER_DIGITAL_R2,   // Spinner Keybind (Reversed)
                pros::E_CONTROLLER_DIGITAL_A     // Control Reverse Toggle
            };

            break;
        default:
            break;
        }
    }
};