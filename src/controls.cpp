#include "include.cpp"
#include "motors.cpp"

pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
bool held = false; // Held tracks the current E-CONTROLLER_DIGITAL_A state


void control_listener() {
        // Sets the motors to the values of the analog sticks. Calls the motorSpeed function from motors.cpp
		motorSpeed(1, master.get_analog(ANALOG_LEFT_Y));
		motorSpeed(2, master.get_analog(ANALOG_RIGHT_Y));
        
		// Toggle-able flywheel logic (Hold DIGITAL_A on the controller to activate/deactivate the flywheel)
        switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            case true: // When Controller Digital A is pushed
                // Uses held to track changes, compares it based off the Controller_Digital_A value recorded
                if (!held) {
                    // Toggles motor using motorCheck() tracking
                    motorSpeed(3,  !(motorCheck(3) == 127) * 127);  
                }
                held = true; // Sets comparison tracker "held" to true
            case false: // When Controller Digital A is not pushed
                held = false; // Sets comparison tracker "held" to false
            default:
                return; // Returns if an erroneous values are detected. Should never happen, since digital outputs should be a 0 or 1.
        }
        /* -- Branchless code: Deprecated, may not function. This was replaced by the above code and will be removed once the
        above code is tested prior to testing --> main pull. --

		held = master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1 && !held;
        
        // If digital controller A is pressed and held. Set flywheel state to 
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 0 && held) motorSpeed(3, !flywheelstate * 127);*/
}