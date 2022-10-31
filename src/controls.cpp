#include "include.cpp"
#include "motors.cpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
bool held = false; // Held tracks the current E-CONTROLLER_DIGITAL_A state


void control_listener() {
        // Sets the motors to the values of the analog sticks. Calls the motorSpeed function from motors.cpp
		motorSpeed(1, master.get_analog(ANALOG_LEFT_Y));
		motorSpeed(2, master.get_analog(ANALOG_RIGHT_Y));
        
		// Toggle-able flywheel logic (Hold DIGITAL_A on the controller to activate/deactivate the flywheel)
        // Set held to true if digital controller A is pressed and held is false
        
        switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            case true: // When Controller Digital A is pushed

                // Compares these two, they will be the same only when the state changes. Changes it to the opposite
                // of the flywheel state and toggles the flywheel to the opposite of the current flywheel state.
                if (!held) {
                    motorSpeed(3,  !motorCheck(3) * 127);  
                }
                held = true;
            case false: // When Controller Digital A is not pushed
                held = false; /*
                if (held_init == held) motorSpeed(3, !flywheelstate * 127);*/
            default:
                return;
        }
        /*
		held = master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1 && !held;
        
        // If digital controller A is pressed and held. Set flywheel state to 
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 0 && held) motorSpeed(3, !flywheelstate * 127);*/
}