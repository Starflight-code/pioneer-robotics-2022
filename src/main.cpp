#include "absolute_positioning.cpp"
#include "controls.cpp"
#include "display/lv_conf.h"
#include "display/lv_draw/lv_draw_rect.h"
#include "display/lv_misc/lv_color.h"
#include "include.cpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <array>
#include <vector>

// #include "variables.cpp"

// Controls listener task
void controls_container(void* params) {
    // Motor_Class Motors = *(Motor_Class*)params;
    Motor_Class Motors;
    cl Control_Listener((Motor_Class)Motors);
    while(true) {
        Control_Listener.controls();
        pros::c::delay(50);
    }
}
void event_listener_container(void* params) {
    // cl Control_Listener;
    while(true) {
        //    Control_Listener.run();
        pros::c::delay(50);
    }
}
/* Testing code for dynamic UI with touchscreen inputs, ignore for now.
lv_obj_t *myButton;
lv_obj_t *myButtonLabel;
lv_obj_t *myLabel;

lv_style_t myButtonStyleREL; // relesed style
lv_style_t myButtonStylePR;  // pressed style

static lv_res_t btn_click_action(lv_obj_t *btn) {
  uint8_t id =
      lv_obj_get_free_num(btn); // id usefull when there are multiple buttons

  if (id == 0) {
    char buffer[100];
    sprintf(buffer, "button was clicked %i milliseconds from start",
            pros::millis());
    lv_label_set_text(myLabel, buffer);
  }

  return LV_RES_OK;
}

void initialize_btn() {
  lv_style_copy(&myButtonStyleREL, &lv_style_plain);
  myButtonStyleREL.body.main_color = LV_COLOR_MAKE(150, 0, 0);
  myButtonStyleREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
  myButtonStyleREL.body.radius = 0;
  myButtonStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

  lv_style_copy(&myButtonStylePR, &lv_style_plain);
  myButtonStylePR.body.main_color = LV_COLOR_MAKE(255, 0, 0);
  myButtonStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
  myButtonStylePR.body.radius = 0;
  myButtonStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);

  myButton = lv_btn_create(
      lv_scr_act(),
      NULL); // create button, lv_scr_act() is deafult screen object
  lv_obj_set_free_num(myButton, 0); // set button is to 0
  lv_btn_set_action(
      myButton, LV_BTN_ACTION_CLICK,
      btn_click_action); // set function to be called on button click
  lv_btn_set_style(myButton, LV_BTN_STYLE_REL,
                   &myButtonStyleREL); // set the relesed style
  lv_btn_set_style(myButton, LV_BTN_STYLE_PR,
                   &myButtonStylePR); // set the pressed style
  lv_obj_set_size(myButton, 200, 50); // set the button size
  lv_obj_align(myButton, NULL, LV_ALIGN_IN_TOP_LEFT, 10,
               10); // set the position to top mid

  myButtonLabel = lv_label_create(
      myButton, NULL); // create label and puts it inside of the button
  lv_label_set_text(myButtonLabel, "Click the Button"); // sets label text

  myLabel = lv_label_create(lv_scr_act(),
                            NULL); // create label and puts it on the screen
  lv_label_set_text(myLabel,
                    "Button has not been clicked yet"); // sets label text
  lv_obj_align(myLabel, NULL, LV_ALIGN_IN_LEFT_MID, 10,
               0); // set the position to center
}
*/
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
    // pros::lcd::initialize();
    //  pros::lcd::initialize();
    //  pros::Motor L1(8, true);
    //  L1 = 90;
    //  pros::delay(5000);
    //  L1 = 0;
    //  initialize_btn();
    //   Sets the background to the image contained within waifu_elijah.c
    /* -- Removed photo, screen will be blank. Replace with a logo once created.*/
    /*lv_obj_t* background = lv_img_create(lv_scr_act(), NULL);
    LV_IMG_DECLARE(waifu_elijah);
    lv_img_set_src(background, &waifu_elijah);
    lv_obj_set_size(background, 480, 240);
    lv_obj_align(background, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);*/
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
void autonomous() {
    double Move_Dist = 1;
    double Init_Pos;
    Motor_Class Motors;
    Motors.leftMotors.set(30);
    Motors.rightMotors.set(30);
    pros::delay(1000); // Sets motors to quarter speed to prevent slipping from the intial acceleration
    Motors.leftMotors.set(-60);
    Motors.rightMotors.set(60);
    pros::delay(4500);         // Move robot forward at ~half speed for 4.5 seconds
    Motors.leftMotors.set(10); // Decreases the drive speed and spins the spinner wheel at ~half speed
    Motors.rightMotors.set(10);
    Motors.spinnerMotors.set(60);
    // pros::delay(1000); // If we can't get encoders to work, using a delay would be inaccurate but acceptable as a last resort
    while(abs(Motors.spinnerMotors.getFastPosition() - Init_Pos) < Move_Dist) {
        pros::delay(50);
    } // Holds up further execution until distance is reached/exceeded (Lower speed if it's exceeded by an unacceptable degree)
    Motors.leftMotors.set(0);
    Motors.rightMotors.set(0);
    Motors.spinnerMotors.set(0); // Stop motors and halts autonomous
}
/*
Psudocode

Dump all disks in corner || try to shoot && fetch a few disks on each bot before manual starts
Dimensions 12 ft by 12 ft field
Disk locations: UNKNOWN
Initial Position: UNKNOWN

Scaled down Psudocode for first comp.

Drive forward 1/2 speed for 3-5 seconds
Spin the spinner, using the color sensor to reach a proper color. Await color sensor implimentation by build team.
*/
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
std::array<uint32_t, 2> cycleRun(uint32_t currentTime, int taskIndex, std::array<uint32_t, 2> milis_cycle, int desiredWaitTime) {
    milis_cycle[taskIndex - 1 < 0 ? milis_cycle.size() - 1 : taskIndex - 1] = (uint32_t)pros::millis; // Prevents the index from reaching -1, instead setting it to the max index value if it is
                                                                                                      // Sets current time as the finishing time of the last task
    int waitTime = desiredWaitTime - (currentTime - milis_cycle[taskIndex]);                          // Calculates current wait time by checking desiredWaitTime(default:50) - time passed since task finished

    pros::c::delay(waitTime < 0 ? 0 : waitTime); // Waits for the wait time, if it is still positive (has not passed)
    return milis_cycle;                          // Sends the updated array back, allows persistance between method executions
}
void opcontrol() {
    std::array<uint32_t, 2> milis_cycle = {0, 0}; // Increase array size when adding new tasks (only up to current amount of tasks)
    int desiredWaitTime = 50;                     // per task wait in milliseconds
    Motor_Class Motors;
    cl Control_Listener(Motors); // Control Listener Initialization
    if(Motors.Robot.task_scheduler) {
        // pros::Task controls(controls_container, (void*)Motors); // control code that interacts with the PROS scheduler
        // pros::Task events(event_listener_container);            // event listener code that interacts with the PROS scheduler
        while(true) {

            milis_cycle = cycleRun((uint32_t)pros::millis, 0, milis_cycle, desiredWaitTime); // Should be placed before the task
            Control_Listener.controls();                                                     // Task 0

            milis_cycle = cycleRun((uint32_t)pros::millis, 1, milis_cycle, desiredWaitTime);
            Control_Listener.run(); // Task 1
        }
    } else {
        while(true) {
            Control_Listener.controls();
            Control_Listener.run();
            pros::c::delay(50); // Waits 50 milliseconds and gives CPU some time to sleep. Increase this
                                // value if the CPU overheats.
        }
    }
}
