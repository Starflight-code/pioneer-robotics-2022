#include "include.cpp"
#include "motors.cpp"
pros::Controller master(pros::E_CONTROLLER_MASTER);
bool held = false;
bool held_init;
bool flywheelstate = false;


void control_listener() {
        // Sets the motors to the values of the analog sticks. Calls the motorSpeed function from motors.cpp
		motorSpeed(1, master.get_analog(ANALOG_LEFT_Y));
		motorSpeed(2, master.get_analog(ANALOG_RIGHT_Y));
        
		// Toggle-able flywheel logic (Hold DIGITAL_A on the controller to activate/deactivate the flywheel)
        // Set held to true if digital controller A is pressed and held is false
        
        switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            case true:
            held_init = !held;
            held = true;
            if (held_init == held) flywheelstate = !flywheelstate; motorSpeed(3, !flywheelstate * 127);

            case false:
            held_init = !held;
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