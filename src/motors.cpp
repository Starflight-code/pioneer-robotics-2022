#include "pros/motors.hpp"
#include "include.cpp"

class Motor_Class {
private:
  float modifier = 1;
  int left_motors = 0;
  int right_motors = 0;
  int flywheels = 0;

public:
  void setModifier(float x) { modifier = x; }
  void setSpeed(int motorSet, int speed) {
// -- Motor Import/Config --
    pros::Motor front_left(2, false); // Motor: Normal
    pros::Motor front_right(1, true); // Motor: Reversed
    pros::Motor rear_left(12, false); // Motor: Normal
    pros::Motor rear_right(11, true); // Motor: Reversed
    /* Uncomment and set ports to enable the 8 motor drive
    pros::Motor front_left_2(x,true); // Motor: Reversed
    pros::Motor front_right_2(x,false); // Motor: Normal
    pros::Motor rear_left_2(x,true); // Motor: Reversed
    pros::Motor rear_right_2(x,false); // Motor: Normal */
    pros::Motor flywheel(3, false);
    pros::Motor flywheel_2(4, true);
// -- END OF MOTOR IMPORT/CONFIG
    switch (motorSet) {

    case 1: // motorSpeed(1, speed) to set left motor group's speed
      front_left = speed * modifier;
      rear_left = speed * modifier;
      /* Uncomment for 8 motor drive
      front_left_2 = speed * modifier;
      rear_left_2 = speed * modifier; */
      left_motors = speed;
      break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
      front_right = speed * modifier;
      rear_right = speed * modifier;
      /* Uncomment for 8 motor drive
      front_right_2 = speed * modifier;
      rear_right_2 = speed * modifier; */
      right_motors = speed;
      break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
      flywheel = speed;
      flywheel_2 = speed;
      flywheels = speed;
      break;
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
    default:
      return 0; // Returns ZERO if an erroneous input was given.
    }
  }
};