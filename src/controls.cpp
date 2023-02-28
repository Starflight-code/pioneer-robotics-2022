#include "PID.cpp"
#include "algorithms.cpp"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "pros/vision.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <vector>
/*
class event {
private:
    pros::Controller* master; // Sets up a pointer to controller master, will be configured by the constructor
    bool tracker;
    pros::controller_digital_e_t button;
    bool execution_hold;
    int hold_index;
    bool reverse_check;
    std::vector<MotorGroup> Motors;

public:
    event(pros::controller_digital_e_t button_to_watch, bool hold_up_execution, MotorGroup& MG, bool reverse = false) {
        pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
        reverse_check = reverse;
        button = button_to_watch;
        execution_hold = hold_up_execution;
        Motors.push_back(MG);
        if(not execution_hold) {
            hold_index = 0;
        } // Turns bool into True: 1 or False: 0 (For a switch statement)
        else {
            hold_index = 1;
        }
    }
    void check() {
        switch(hold_index) {
        case 0:
            if(reverse_check ? not button : button) { // Set the speed to 0 be default, and 170 upon button press when reverse_check is false
                Motors[0].set(0);
            } else {
                Motors[0].set(170);
            }
            break;
        case 1:
            while(reverse_check ? not button : button) {
                Motors[0].set(0);
                pros::delay(50);
            }
            Motors[0].set(170);
            break;
        }
    }
};*/

class cl {
private:
    double limiter;                      // Global limiter value (double w/ range [0 <-> 1])
    double sc = 0;                       // Scaling constant, for use with arcade control system
    const int _RANGE = 127;              // Range of vex motors/controls (max value allowed)
    const int _SPINNER_SPEED = 60;       // Speed that spinner will spin at
    const int _SPINNER_DRIVE_SPEED = 10; // Speed for drive motors to push robot
                                         // forward, keeping contact with spinner
    bool spinnerActive = false;          // Allows for control while pushing robot forward
                                         // for spinner (Quality of Life)
    Control Flywheels;                   // PID and other control alogrithms
public:
    Motor_Class Motors;
    algorithms algo;
    cl() {
        limiter = Motors.Robot.limiter;
    }

private:
    /// Scales control system to x^1.5 (by default), x is between 0 and 1 (speed should look like an exponential curve maxing out at 170).
    /// Created to improve low speed precision while perserving access to high speed settings.
    /*
    int exponential_control(int controlInput, double exponent) {
        int negativeCarry = (controlInput < 0) * -1; // Carrys the negative, would otherwise be lost during exponent calcualtion
        return negativeCarry * round(_RANGE * pow((controlInput / _RANGE), exponent));
    }
    void tank_control(int control_input_left, int control_input_right) {
        Motors.setSpeed(1, int(control_input_left * (limiter)));
        Motors.setSpeed(2, int(control_input_right * (limiter)));
    }
    void arcade_control(int controlInput_y, int controlInput_x) {
        // Designed to allow a mix of left/right forward/backwards inputs, so
        // the scaling coefficient prevents overflow (values >170)
        double sc = (abs(controlInput_y) +
                     abs(controlInput_x));
        if(sc < _RANGE) {
            sc = _RANGE;
        }
        double scalingConstant = sc / _RANGE;
        // If it isn't limiting the value (making the value larger instead),
        // set it to do nothing (multiply by 1 (127/127))

        // Sets the motors to the values generated by the LEFT_Y and RIGHT_X
        // stick ranges Left_Y stick controls forward/backwards Right_X stick
        // controls left/right and local limiter still works
        Motors.setSpeed(
            1, int((-(controlInput_y / scalingConstant +
                      controlInput_x / scalingConstant) *
                    (limiter))));
        Motors.setSpeed(
            2, int(((controlInput_y / scalingConstant -
                     controlInput_x / scalingConstant) *
                    (limiter))));
    }*/

    /// Applies motor speeds following the preset control scheme for the drive.
    void controls() {

        std::array<int, 2> controller_values;
        std::array<pros::controller_analog_e_t, 2> sticks;

        pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
        // Set sticks arrays to correct values for current configuration
        if(Motors.Robot.controlScheme == 0) {
            sticks = {ANALOG_LEFT_Y, ANALOG_RIGHT_Y}; // Tank control
        } else {
            sticks = {ANALOG_LEFT_Y, ANALOG_RIGHT_X}; // Arcade Control
        }

        if(Motors.Robot.exponential_control) { // Apply exponential control altering and populate controller_values array
            for(int i = 0; i < controller_values.size(); i++) {
                controller_values[i] = algo.exponential_control(master.get_analog(sticks[i]), Motors.Robot.control_exponent_value);
            }
        } else {
            for(int i = 0; i < controller_values.size(); i++) { // Populate controller_values
                                                                // array with raw stick values
                controller_values[i] = master.get_analog(sticks[i]);
            }
        }

        Motors.setSpeed(1, controller_values[0]);
        // pros::lcd::print(2, (const char*)(controller_values[0] + controller_values[1]));
        Motors.setSpeed(2, controller_values[1]);
        // Motors.setSpeed(1, 50);
        // pros::delay(200);
        /*
                if(Motors.Robot.exponential_control) {   // Prepares controller values, either processed via the exponential control system or places the raw data
                    switch(Motors.Robot.controlScheme) { // into the controller_values array. Allowing them to be used to set speeds following the control scheme preset.
                    case 0:                              // Tank control
                        controller_values[0] = exponential_control(master.get_analog(ANALOG_LEFT_Y), Motors.Robot.control_exponent_value);
                        controller_values[1] = exponential_control(master.get_analog(ANALOG_RIGHT_Y), Motors.Robot.control_exponent_value);
                        break;
                    case 1: // Arcade control
                        controller_values[0] = exponential_control(master.get_analog(ANALOG_LEFT_Y), Motors.Robot.control_exponent_value);
                        controller_values[1] = exponential_control(master.get_analog(ANALOG_RIGHT_X), Motors.Robot.control_exponent_value);
                        break;
                    }
                } else {
                    switch(Motors.Robot.controlScheme) {
                    case 0:
                        controller_values[0] = master.get_analog(ANALOG_LEFT_Y);
                        controller_values[1] = master.get_analog(ANALOG_RIGHT_Y);
                        break;
                    case 1:
                        controller_values[0] = master.get_analog(ANALOG_LEFT_Y);
                        controller_values[1] = master.get_analog(ANALOG_RIGHT_X);
                        break;
                    }
                }*/
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

        switch(Motors.Robot.controlScheme) {
        case 0: // Tank control --- REMOVE THIS CODE AND ALL (COMMENTED OUT) CONTROL SCHEME CODE ONCE FUNCTIONALITY OF MODULAR SYSTEM CAN BE VERIFIED ---
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
            for(int i = 0; i < controller_values.size(); i++) {
                Motors.Motors[i].set(algo.tank_control(controller_values[i], limiter));
            }

            // tank_control(controller_values[0], controller_values[1]);
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
            std::array<int, 2> motorValues;
            motorValues = algo.arcade_control(controller_values[0], controller_values[1], limiter);
            for(int i = 0; i < motorValues.size(); i++) {
                Motors.Motors[i].set(motorValues[i]);
            }
            //}
            break;
        default:
            // SHOULD NEVER OCCUR, but if it does...
            // There is an error in variables.cpp in
            // the Robot.controlScheme variable preset
            break;
        }
    }
    void event_listener() {
    }
    /// Control Listening Service
    ///
    /// Pulls from control inputs based on robot.cpp's config.
    /// Sends calculated output to motors.
public:
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
                    /*
                    if(master.get_digital(DIGITAL_L1)) {
                        spinnerActive = true;
                        Motors.setSpeed(4, _SPINNER_SPEED);
                    } else if(master.get_digital(DIGITAL_L2)) {
                        spinnerActive = true;
                        Motors.setSpeed(4, -_SPINNER_SPEED);
                    } else {
                        spinnerActive = false;
                        Motors.setSpeed(4, 0);
                    }
                    */
        if(Motors.Robot.training and
           master.get_digital(
               pros::E_CONTROLLER_DIGITAL_LEFT)) { // Calculates local limmiter
                                                   // from left stick's x value,
                                                   // uses the full stick range.
            limiter = (double)(master.get_analog(ANALOG_LEFT_X) + _RANGE) / 254;
        }

        if(master.get_digital(DIGITAL_R1)) { // Toggles on button press, doesn't
                                             // hold up the control loop

            if(not(R1_State == (Motors.getSpeed(3) == _RANGE))) {
                R1_State = !R1_State;
            } // If motors are active and that's not the same as R1's state, it has
              // changed
            Motors.setSpeed(3, R1_State * _RANGE);
        }
        if(master.get_digital(DIGITAL_R2)) { // Toggles on button press, doesn't
                                             // hold up the control loop

            if(not(R2_State == (Motors.getSpeed(3) == _RANGE))) {
                R2_State = !R2_State;
            } // If motors are active and that's not the same as R1's state, it has
              // changed
            Motors.setSpeed(3, R2_State * _RANGE);
        }
        // REPLACE W/ Modular systems possibly in an event listener file, impliment in a future update

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