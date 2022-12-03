#include "pros/motors.hpp"
#include "include.cpp"

class Motor_Class {
private:
  float modifier = 1;
  int left_motors = 0;
  int right_motors = 0;
  int flywheels = 0;
  int spinner_speed = 0;

public:
  void setModifier(float x) { modifier = x; }
  void setSpeed(int motorSet, int speed) {
// -- Motor Import/Config --
    /*pros::Motor L1(9, true); // Motor L1: Normal
    pros::Motor L2(5,false);  // Motor L2: Reversed
    pros::Motor L3(4, true);   // Motor L3: Normal
    pros::Motor L4(8,false);   // Motor L4: Reversed

    pros::Motor R1(20, false);   // Motor R1: Reversed
    pros::Motor R2(19,true); // Motor R2: Normal
    pros::Motor R3(18, false);    // Motor R3: Reversed
    pros::Motor R4(17,true);  // Motor R4: Normal

    pros::Motor flywheel(3, false);
    pros::Motor flywheel_2(4, true);
    pros::Motor spinner(16, false);
    */

    pros::Motor L1(16, true); // Motor L1: Normal
    pros::Motor L2(6,false);  // Motor L2: Reversed
    pros::Motor L3(3, true);   // Motor L3: Normal
    pros::Motor L4(8,false);   // Motor L4: Reversed

    pros::Motor R1(20, false);   // Motor R1: Reversed
    pros::Motor R2(19,true); // Motor R2: Normal
    pros::Motor R3(18, false);    // Motor R3: Reversed
    pros::Motor R4(17,true);  // Motor R4: Normal

    pros::Motor flywheel(3, false);
    pros::Motor flywheel_2(4, true);
    pros::Motor spinner(13,false);
// -- END OF MOTOR IMPORT/CONFIG
    switch (motorSet) {

    case 1: // motorSpeed(1, speed) to set left motor group's speed
      L1 = speed * modifier;
      L2 = speed * modifier;
      L3 = speed * modifier;
      L4 = speed * modifier;
      left_motors = speed;
      break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
      R1 = speed * modifier;
      R2 = speed * modifier;
      R3 = speed * modifier;
      R4 = speed * modifier;
      right_motors = speed;
      break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
      flywheel = speed;
      flywheel_2 = speed;
      flywheels = speed;
      break;
      case 4:
      spinner = speed;
      spinner_speed = speed;
    default:
      return;
    }
    return;
  };
  int getSpeed(int motorSet) {
    switch (motorSet) {
    case 1:
      return left_motors; // Returns the left motor group tracker value (speed
                          // of the motor set)
    case 2:
      return right_motors; // Returns the right motor group tracker value (speed
                           // of the motor set)
    case 3:
      return flywheels; // Returns the flywheel motor group tracker value (speed
                        // of the motor set)
    case 4:
      return spinner_speed;
    default:
      return 0; // Returns ZERO if an erroneous input was given.
    }
  }
};