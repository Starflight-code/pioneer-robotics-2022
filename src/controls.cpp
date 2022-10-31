#include "include.cpp"
#include "motors.cpp"
pros::Controller master(pros::E_CONTROLLER_MASTER);
// Test above to see if they persist between function calls and stay within scope of control_listener
//bool held;
//bool flywheelstate;


void control_listener(bool held, bool flywheelstate) {
        // Sets the motors to the values of the analog sticks. Calls the motorSpeed function from motors.cpp
		motorSpeed(1, master.get_analog(ANALOG_LEFT_Y));
		motorSpeed(2, master.get_analog(ANALOG_RIGHT_Y));
        
		// Toggle-able flywheel logic (Hold DIGITAL_A on the controller to activate/deactivate the flywheel)
		held = master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1 && !held;

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 0 && held) motorSpeed(3, !flywheelstate * 127); 
}