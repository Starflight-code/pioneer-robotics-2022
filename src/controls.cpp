#include "PID.cpp"

class cl {
private:
  double local_limiter = 100;

public:
  void run() {

    Motor_Class Motors;
    Control Flywheels;
    pros::Controller master(pros::E_CONTROLLER_MASTER); // Imports Controller as "master"
    bool held_R1 = false; // Held tracks the current E-CONTROLLER_DIGITAL_R1
                          // state; Halfcourt
    bool held_R2 = false; // Held tracks the current E-CONTROLLER_DIGITAL_R2
                          // state; Point-Blank

    /* Tracks flywheel state 0: off 1: Halfcourt 2: Point-Blank
       Sets the motors to the values of the analog sticks. Calls the motorSpeed
       function from motors.cpp  */
       int flywheelState = 0; 
    
    master.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      local_limiter = (master.get_analog(ANALOG_LEFT_X) + 170) / 3.4;
    }
    Motors.setSpeed(
        1, int(master.get_analog(ANALOG_LEFT_Y) * (local_limiter / 100)));
    Motors.setSpeed(
        2, int(master.get_analog(ANALOG_RIGHT_Y) * (local_limiter / 100)));

    // Toggle-able flywheel logic (Hold DIGITAL_A on the controller to
    // activate/deactivate the flywheel)
    switch (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
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
    }
  }
};