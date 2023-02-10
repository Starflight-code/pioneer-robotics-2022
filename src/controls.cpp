#include "PID.cpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include <cmath>

class cl {
private:
  double local_limiter = 100;
  double sc = 0;
  const int _RANGE = 127; // Range of vex motors/controls (max value allowed)
  const int _SPINNER_SPEED = 60;       // Speed that spinner will spin at
  const int _SPINNER_DRIVE_SPEED = 10; // Speed for drive motors to push robot
                                       // forward, keeping contact with spinner
  bool spinnerActive = false; // Allows for control while pushing robot forward
                              // for spinner (Quality of Life)
  Control Flywheels;          // PID and other control alogrithms
  int exponential_control(int controlInput, double exponent) {
    int negativeCarry = (controlInput < 0) * -1; // Carrys the negative, would otherwise be lost during exponent calcualtion
    return negativeCarry * round(_RANGE * pow((controlInput / _RANGE), exponent));
  }
  void tank_control(int controlInput) {
    Motors.setSpeed(
            1, int(controlInput * (local_limiter / 100)));
        Motors.setSpeed(
            2, int(controlInput * (local_limiter / 100)));
  }
  void arcade_control(int controlInput_y, int controlInput_x) {
        // Designed to allow a mix of left/right forward/backwards inputs, so
        // the scaling coefficient prevents overflow (values >170)
        double sc = (abs(controlInput_y) +
              abs(controlInput_x));
        if (sc < _RANGE) {sc = _RANGE;}
        double scalingConstant = sc / _RANGE;
         // If it isn't limiting the value (making the value larger instead),
          // set it to do nothing (multiply by 1 (127/127))

        // Sets the motors to the values generated by the LEFT_Y and RIGHT_X
        // stick ranges Left_Y stick controls forward/backwards Right_X stick
        // controls left/right and local limiter still works
        Motors.setSpeed(
            1, int((-(controlInput_y / scalingConstant +
                      controlInput_x / scalingConstant) *
                    (local_limiter / 100))));
        Motors.setSpeed(
            2, int(((controlInput_y / scalingConstant -
                     controlInput_x / scalingConstant) *
                    (local_limiter / 100))));
  }
  
public:
  Motor_Class Motors;
  /// Scales control system to x^1.5 (by default), x is between 0 and 1 (speed should look like an exponential curve maxing out at 170)
  
  
  void controls() {
    /// 0: ANALOG_LEFT_Y value 1: ANALOG_RIGHT_X (if applicable)
    int controller_values[2];
    //int controller_value;
    //int controller_x_value;
    pros::Controller master(
        pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
    
    if (Motors.Robot.exponential_control){
      switch(Motors.Robot.controlScheme) {
      case 0:
      controller_values[0] = exponential_control(master.get_analog(ANALOG_LEFT_Y), Motors.Robot.control_exponent_value);
      break;
      case 1:
      controller_values[0] = exponential_control(master.get_analog(ANALOG_LEFT_Y), Motors.Robot.control_exponent_value);
      controller_values[1] = exponential_control(master.get_analog(ANALOG_RIGHT_X), Motors.Robot.control_exponent_value);
      break;
    }} else {
      switch (Motors.Robot.controlScheme) {
        case 0:
        controller_values[0] = master.get_analog(ANALOG_LEFT_Y);
        break;
        case 1:
        controller_values[0] = master.get_analog(ANALOG_LEFT_Y);
        controller_values[1] = master.get_analog(ANALOG_RIGHT_X);
        break;
      }
    }
/*
    if (Motors.Robot.exponential_control) { // Scales control system to x^1.5 (by default), x is between 0 and 1 (speed should look like an exponential curve maxing out at 170)
    //int negativeCarry = (master.get_analog(ANALOG_LEFT_Y) < 0) * -1; // Carrys the negative, would otherwise be lost during exponent calcualtion
    //controller_value = negativeCarry * round(_RANGE * pow((master.get_analog(ANALOG_LEFT_Y) / _RANGE), Motors.Robot.control_exponent_value));
    controller_values[0] = exponential_control(master.get_analog(ANALOG_LEFT_Y), Motors.Robot.control_exponent_value);
    if (Motors.Robot.controlScheme == 1) { // Adds calculation for x stick if arcade control is being used
      //int negativeCarry_x = (master.get_analog(ANALOG_RIGHT_X) < 0) * -1;
      //controller_x_value = negativeCarry * round(_RANGE * pow((master.get_analog(ANALOG_RIGHT_X) / _RANGE), Motors.Robot.control_exponent_value));
      controller_values[1] = exponential_control(master.get_analog(ANALOG_RIGHT_X), Motors.Robot.control_exponent_value);
    }
    } else {
      controller_value = master.get_analog(ANALOG_LEFT_Y);
      if (Motors.Robot.controlScheme == 1) {
        controller_x_value = master.get_analog(ANALOG_RIGHT_X);
      }
    }*/

    switch (Motors.Robot.controlScheme) {
    case 0: // Tank control
        /*if (-1 < (controller_value) and
          ((controller_value) < 1) and (spinnerActive)) {
        Motors.setSpeed(1, _SPINNER_DRIVE_SPEED);
        Motors.setSpeed(2, _SPINNER_DRIVE_SPEED);
      } else {*/

        /*Motors.setSpeed(
            1, int(controller_value * (local_limiter / 100)));
        Motors.setSpeed(
            2, int(controller_value * (local_limiter / 100)));*/
      //}
      tank_control(controller_values[0]);
      break;
    case 1: // Split Arcade

        /*if (-1 < (controller_value) and
          (controller_value < 1) and spinnerActive) {
        Motors.setSpeed(1, _SPINNER_DRIVE_SPEED);
        Motors.setSpeed(2, _SPINNER_DRIVE_SPEED);
      } else {*/
        // Designed to allow a mix of left/right forward/backwards inputs, so
        // the scaling coefficient prevents overflow (values >170)
        /*sc = (abs(controller_value) +
              abs(controller_x_value));
        if (sc < _RANGE) {sc = _RANGE;}*/
         // If it isn't limiting the value (making the value larger instead),
          // set it to do nothing (multiply by 1 (127/127))

        // Sets the motors to the values generated by the LEFT_Y and RIGHT_X
        // stick ranges Left_Y stick controls forward/backwards Right_X stick
        // controls left/right and local limiter still works
        /*Motors.setSpeed(
            1, int((-(controller_value / (sc / _RANGE) +
                      controller_x_value / (sc / _RANGE)) *
                    (local_limiter / 100))));
        Motors.setSpeed(
            2, int(((controller_value / (sc / _RANGE) -
                     controller_x_value / (sc / _RANGE)) *
                    (local_limiter / 100))));*/
      arcade_control(controller_values[0], controller_values[1]);
      //}
      break;
    default:
      // SHOULD NEVER OCCUR, but if it does...
      // There is an error in variables.cpp in 
      // the Robot.controlScheme variable preset
      break;
    }
  }
  void run() {

    pros::Controller master(
        pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
    // bool held_R1 = false; // Held tracks the current E-CONTROLLER_DIGITAL_R1
    //  state; Halfcourt
    // bool held_R2 = false; // Held tracks the current E-CONTROLLER_DIGITAL_R2
    //  state; Point-Blank
    bool R1_State = false; // Tracker allowing for R1 toggle to function
    bool R2_State = false; // Tracker allowing for R2 toggle to function

    /* Tracks flywheel state 0: off 1: Halfcourt 2: Point-Blank
       Sets the motors to the values of the analog sticks. Calls the motorSpeed
       function from motors.cpp  */
    // int flywheelState = 0; /PART OF DEPRECATED CODE, pending
    // re-implimentation. See message below.

    controls(); // Applies motor speeds following the preset control scheme for
                // the drive

    if (master.get_digital(DIGITAL_L1)) {
      spinnerActive = true;
      Motors.setSpeed(4, _SPINNER_SPEED);
    } else if (master.get_digital(DIGITAL_L2)) {
      spinnerActive = true;
      Motors.setSpeed(4, -_SPINNER_SPEED);
    } else {
      spinnerActive = false;
      Motors.setSpeed(4, 0);
    }
    if (Motors.Robot.training and
        master.get_digital(
            pros::E_CONTROLLER_DIGITAL_LEFT)) { // Calculates local limmiter
                                                // from left stick's x value,
                                                // uses the full stick range.
      local_limiter = (master.get_analog(ANALOG_LEFT_X) + _RANGE) / 2.54;
    }

    if (master.get_digital(DIGITAL_R1)) { // Toggles on button press, doesn't
                                          // hold up the control loop

      if (not(R1_State == (Motors.getSpeed(3) == _RANGE))) {
        R1_State = !R1_State;
      } // If motors are active and that's not the same as R1's state, it has
        // changed
      Motors.setSpeed(3, R1_State * _RANGE);
    }
    if (master.get_digital(DIGITAL_R2)) { // Toggles on button press, doesn't
                                          // hold up the control loop

      if (not(R2_State == (Motors.getSpeed(3) == _RANGE))) {
        R2_State = !R2_State;
      } // If motors are active and that's not the same as R1's state, it has
        // changed
      Motors.setSpeed(3, R2_State * _RANGE);
    }
    // Toggle-able flywheel logic (Hold DIGITAL_A on the controller to
    // --DEPRECATED-- pending re-implimentation in a less complex way
    // activate/deactivate the flywheel)
    /*switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    case true: // When Controller Digital A is pushed
      // Uses held to track changes, compares it based off the
      // Controller_Digital_A value recorded
      if (!held_R1) {
        switch (flywheelState) {
        case 0:
          Motors.setSpeed(3, Flywheels.PID(3600));
          flywheelState = 1;
          break;
        case 1:
          Motors.setSpeed(3, 0);
          flywheelState = 0;
          break;
        case 2:
          Motors.setSpeed(3, Flywheels.PID(3600));
          flywheelState = 1;
          break;
        }
      }
      held_R1 = true;  // Sets comparison tracker "held" to true
    case false:        // When Controller Digital A is not pushed
      held_R1 = false; // Sets comparison tracker "held" to false
    default:
      return; // Returns if an erroneous values are detected. Should never
              // happen, since digital outputs should be a 0 or 1.
    }
    switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    case true: // When Controller Digital A is pushed
      // Uses held to track changes, compares it based off the
      // Controller_Digital_A value recorded
      if (!held_R2) {
        // Toggles motor using motorCheck() tracking
        switch (flywheelState) {
        case 0:
          Motors.setSpeed(3, Flywheels.PID(2000));
          flywheelState = 2;
          break;
        case 1:
          Motors.setSpeed(3, Flywheels.PID(2000));
          flywheelState = 2;
          break;
        case 2:
          Motors.setSpeed(3, 0);
          flywheelState = 0;
          break;
        }
      }
      held_R2 = true;  // Sets comparison tracker "held" to true
    case false:        // When Controller Digital A is not pushed
      held_R2 = false; // Sets comparison tracker "held" to false
    default:
      return; // Returns if an erroneous values are detected. Should never
              // happen, since digital outputs should be a 0 or 1.
    }*/
  }
};