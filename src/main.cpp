#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <iostream>
#include "motors.cpp"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
 /*
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}*/

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	// Sets the background to the image contained within waifu_elijah.c
	
lv_obj_t* background = lv_img_create(lv_scr_act(), NULL);
LV_IMG_DECLARE(raptor_shark);
lv_img_set_src(background, &raptor_shark);
lv_obj_set_size(background, 480, 240);
lv_obj_align(background, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	// Imports the Controller and maps it to the variable master
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	// Imports the motors and maps them to variables
	//pros::Motor motor_name(port);
	/*
	pros::Motor front_left(2,false); // Motor: Normal
	pros::Motor front_right(1,true); // Motor: Reversed
	pros::Motor rear_left(12,false); // Motor: Normal
	pros::Motor rear_right(11,true); // Motor: Reversed
	*/
	/*
	pros::Motor front_left_2(x,true); // Motor: Reversed
	pros::Motor front_right_2(x,false); // Motor: Normal
	pros::Motor rear_left_2(x,true); // Motor: Reversed
	pros::Motor rear_right_2(x,false); // Motor: Normal
	*/
	/*
	pros::Motor flywheel(3);
	pros::Motor flywheel_2(4);
	*/


	//Used within control/logic structures
	bool held;
	bool flywheelstate = false;


	while (true) {
		
	// Test code for randomly setting the screen to different colors. May violate vex rules, do not enable when competing.
	/*
	//pros::screen::set_pen(RGB2COLOR((char) (rand() % 255 + 0),(char)  (rand() % 255 + 0), (char) (rand() % 255 + 0)));
	//pros::screen::fill_rect(0,0,1000,300);
	//pros::screen::erase();
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
*/
		// Sets the motors to the values of the analog sticks. 
		front_left = limiter * master.get_analog(ANALOG_LEFT_Y);
		front_right = limiter * master.get_analog(ANALOG_RIGHT_Y);
		rear_left = limiter * master.get_analog(ANALOG_LEFT_Y);
		rear_right = limiter * master.get_analog(ANALOG_RIGHT_Y);
		/*
		front_left_2 = limiter * master.get_analog(ANALOG_LEFT_Y);
		front_right_2 = limiter * master.get_analog(ANALOG_RIGHT_Y);
		rear_left_2 = limiter * master.get_analog(ANALOG_LEFT_Y);
		rear_right_2 = limiter * master.get_analog(ANALOG_RIGHT_Y);
		*/

		// Toggle-able flywheel logic (Hold DIGITAL_A on the controller to activate/deactivate the flywheel)
		held = master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1 && !held;
		// This is a branching version of the above code. Switch to this only if the branchless version doesn't work. (Branchless code is more efficient)
		/*if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1 && !held) { 
			held = true;
		}*/
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 0 && held) { 
		if (flywheelstate == true) {
		// Flywheel is toggled OFF
		flywheel = 0;
		flywheel_2 = 0;	
		//std::cout << "Flywheel Off";
		} else {
		// Flywhhel is toggled ON
		flywheel = 127;
		flywheel_2 = -127;
		//std::cout << "Flywheel On";
		}
		}
		pros::delay(50);
	}
}
