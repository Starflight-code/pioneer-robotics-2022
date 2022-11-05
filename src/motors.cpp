#include "pros/motors.hpp"
#include "include.cpp"

class Motor_Class {
private:
  float modifier = 1;
  int left_motors = 0;
  int right_motors = 0;
  int flywheels = 0;
  // bool init = false;
  /*pros::Motor rl = rear_left;
  pros::Motor rr = rear_right;
  pros::Motor fl = front_left;
  pros::Motor fr = front_left;*/

public:
  void setModifier(float x) { modifier = x; }
  void init_f() {
    // #include "variables.cpp"
    //  Limits the motor speed, must be 0 <= modifier <= 1
    /*float modifier = 1;
    int left_motors = 0;
    int right_motors = 0;
    int flywheels = 0;
    */
  }
  void setSpeed(int motorSet, int speed) {
    // if (not init) {
    // init = true;
    pros::Motor front_left(2, false); // Motor: Normal
    pros::Motor front_right(1, true); // Motor: Reversed
    pros::Motor rear_left(12, false); // Motor: Normal
    pros::Motor rear_right(11, true); // Motor: Reversed
    /* Uncomment and set ports to enable the 8 motor drive
    pros::Motor front_left_2(x,true); // Motor: Reversed
    pros::Motor front_right_2(x,false); // Motor: Normal
    pros::Motor rear_left_2(x,true); // Motor: Reversed
    pros::Motor rear_right_2(x,false); // Motor: Normal
    */
    pros::Motor flywheel(3, false);
    pros::Motor flywheel_2(4, true);
    //};
    switch (motorSet) {

    case 1: // motorSpeed(1, speed) to set left motor group's speed
      front_left = speed * modifier;
      rear_left = speed * modifier;
      /* Uncomment for 8 motor drive
      front_left_2 = speed * modifier;
      rear_left_2 = speed * modifier;
      */
      left_motors = speed;
      break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
      front_right = speed * modifier;
      rear_right = speed * modifier;
      /* Uncomment for 8 motor drive
      front_right_2 = speed * modifier;
      rear_right_2 = speed * modifier;
      */
      right_motors = speed;
      break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
      flywheel = speed;
      flywheel_2 = speed;
      flywheels = speed;
      break;
    default:
      // throw "Error, invalid input sent to function motorSpeed()";
      return;
      // break;
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