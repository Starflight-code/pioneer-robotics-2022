#include "pros/motors.hpp"
#include "include.cpp"
#include <vector>
// #include <tuple>


class Motor_Class {
private:
  float modifier = 1; // This reduced the motor power by POWER * modifier,
                      // should be between 0 and 1 or it will return an error.
  int left_motors = 0;
  int right_motors = 0;
  int flywheels = 0;
  std::vector<pros::Motor_Group> mtrgroup;

public:
void motorGroup() {
    pros::Motor L1(8, true);  // Motor L1: Normal
    pros::Motor L2(6, false); // Motor L2: Reversed
    pros::Motor L3(5, true);  // Motor L3: Normal
    pros::Motor L4(9, false); // Motor L4: Reversed

    pros::Motor R1(20, false); // Motor R1: Reversed
    pros::Motor R2(19, true);  // Motor R2: Normal
    pros::Motor R3(18, false); // Motor R3: Reversed
    pros::Motor R4(17, true);  // Motor R4: Normal

    pros::Motor F1(3, false);
    pros::Motor F2(4, true);
    pros::Motor_Group Left{L1, L2, L3, L4};
    pros::Motor_Group Right{R1, R2, R3, R4};
    pros::Motor_Group Flywheel{F1, F2};
    mtrgroup.clear();
    mtrgroup.push_back(Left);
    mtrgroup.push_back(Right);
    mtrgroup.push_back(Flywheel);

  }
  void setSpeed(int motorSet, int speed) {
    /*
    pros::Motor L1(8, true);  // Motor L1: Normal
    pros::Motor L2(6, false); // Motor L2: Reversed
    pros::Motor L3(5, true);  // Motor L3: Normal
    pros::Motor L4(9, false); // Motor L4: Reversed

    pros::Motor R1(20, false); // Motor R1: Reversed
    pros::Motor R2(19, true);  // Motor R2: Normal
    pros::Motor R3(18, false); // Motor R3: Reversed
    pros::Motor R4(17, true);  // Motor R4: Normal

    pros::Motor F1(3, false);
    pros::Motor F2(4, true);
    */
    switch (motorSet) {

    case 1: // motorSpeed(1, speed) to set left motor group's speed
      //L1 = speed * modifier;
      //L2 = speed * modifier;
      //L3 = speed * modifier;
      //L4 = speed * modifier;
      mtrgroup[0] = speed * modifier;
      left_motors = speed;
      break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
      //R1 = speed * modifier;
      //R2 = speed * modifier;
      //R3 = speed * modifier;
      //R4 = speed * modifier; 
      
      mtrgroup[1] = speed * modifier;
      right_motors = speed;
      break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
      //F1 = speed;
      //F2 = speed;
      mtrgroup[2];
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
  
  float getVelocity(int motorSet) {
    pros::Motor L1(8, true);  // Motor L1: Normal
    pros::Motor L2(6, false); // Motor L2: Reversed
    pros::Motor L3(5, true);  // Motor L3: Normal
    pros::Motor L4(9, false); // Motor L4: Reversed

    pros::Motor R1(20, false); // Motor R1: Reversed
    pros::Motor R2(19, true);  // Motor R2: Normal
    pros::Motor R3(18, false); // Motor R3: Reversed
    pros::Motor R4(17, true);  // Motor R4: Normal

    pros::Motor F1(3, false);
    pros::Motor F2(4, true);
    switch (motorSet) {
    case 1:
    //std::vector<double> x = mtrgroup[0].get_actual_velocities();
    /*for (i = 0; i < 4;) {

    }*/
      return (L1.get_actual_velocity() + -L2.get_actual_velocity() +
              L3.get_actual_velocity() + -L4.get_actual_velocity()) /
             4; // Returns the left motor group tracker value (speed
                // of the motor set)
    case 2:
      return (R1.get_actual_velocity() + -R2.get_actual_velocity() +
              R3.get_actual_velocity() + -R4.get_actual_velocity()) /
             4; // Returns the right motor group tracker value (speed
                // of the motor set)
    case 3:
      return (F1.get_actual_velocity() + -F2.get_actual_velocity()) /
             2; // Returns the flywheel motor group tracker value (speed
                // of the motor set)
    default:
      return 0;
    }
  }
  float getFastVelocity(int motorSet) {
    pros::Motor L1(8, true); // Motor L1: Normal

    pros::Motor R1(20, false); // Motor R1: Reversed

    pros::Motor F1(3, false);
    switch (motorSet) {
    case 1:
      return L1.get_actual_velocity(); // Returns the left motor group tracker
                                       // value (speed of the motor set)
    case 2:
      return R1.get_actual_velocity(); // Returns the right motor group tracker
                                       // value (speed of the motor set)
    case 3:
      return F1.get_actual_velocity(); // Returns the flywheel motor group
                                       // tracker value (speed of the motor set)
    default:
      return 0;
    }
  }
};