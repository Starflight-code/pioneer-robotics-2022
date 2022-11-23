#include "pros/motors.hpp"
#include "include.cpp"
#include "pros/rtos.hpp"
#include <vector>
// #include <tuple>

class MotorGrp{
  public:
std::vector<pros::Motor> leftMotors;
std::vector<pros::Motor> rightMotors;
std::vector<pros::Motor> flywheelMotors;
//pros::MutexVar<pros::Motor_Group> Left;
//pros::MutexVar<pros::Motor_Group> Right;
//pros::MutexVar<pros::Motor_Group> Flywheel;
void motorGroup() {

    pros::Motor L1(8, true);  // Motor L1: Normal
    pros::Motor L2(6, false); // Motor L2: Reversed
    pros::Motor L3(5, true);  // Motor L3: Normal
    pros::Motor L4(9, false); // Motor L4: Reversed

    pros::Motor R1(20, false); // Motor R1: Reversed
    pros::Motor R2(19, true);  // Motor R2: Normal
    pros::Motor R3(18, false); // Motor R3: Reversed
    pros::Motor R4(17, true);  // Motor R4: Normal

    const pros::Motor F1(3, false);  // Motor F1: Normal   | Flywheel
    const pros::Motor F2(4, true);   // Motor F2: Reversed | Flywheel
    //pros::Motor_Group Left{L1, L2, L3, L4};
    //pros::Motor_Group Right{R1, R2, R3, R4};
    //pros::Motor_Group Flywheel{F1, F2};
    leftMotors.clear();
    leftMotors.push_back(L1);
    leftMotors.push_back(L2);
    leftMotors.push_back(L3);
    leftMotors.push_back(L4);
    
    rightMotors.clear();
    rightMotors.push_back(R1);
    rightMotors.push_back(R2);
    rightMotors.push_back(R3);
    rightMotors.push_back(R4);

    flywheelMotors.clear();
    flywheelMotors.push_back(F1);
    flywheelMotors.push_back(F2);
    }
};
class Motor_Class {
private:
  float modifier = 1; // This reduced the motor power by POWER * modifier,
                      // should be between 0 and 1 or it will return an error.
  int left_motors = 0;
  int right_motors = 0;
  int flywheels = 0;
  bool init = true;
  

public:
MotorGrp motor;
  void setSpeed(int motorSet, int speed) {
    if (init == true) { motor.motorGroup(); }
    init = false;
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
    pros::Motor_Group left{motor.leftMotors[0], motor.leftMotors[1], motor.leftMotors[2], motor.leftMotors[3]};
    pros::Motor_Group right{motor.rightMotors[0], motor.rightMotors[1], motor.rightMotors[2], motor.rightMotors[3]};
    pros::Motor_Group flywheel{motor.flywheelMotors[0], motor.flywheelMotors[1]};
    switch (motorSet) {
    
    
    case 1: // motorSpeed(1, speed) to set left motor group's speed
      //L1 = speed * modifier;
      //L2 = speed * modifier;
      //L3 = speed * modifier;
      //L4 = speed * modifier;
      left = speed * modifier;
      left_motors = speed;
      break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
      //R1 = speed * modifier;
      //R2 = speed * modifier;
      //R3 = speed * modifier;
      //R4 = speed * modifier; 
      right = speed * modifier;
      right_motors = speed;
      break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
      //F1 = speed;
      //F2 = speed;
      flywheel = speed * modifier;
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
    /*pros::Motor L1(8, true);  // Motor L1: Normal
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
double total;
    switch (motorSet) {
    case 1:
    //std::vector<double> x = mtrgroup[0].get_actual_velocities();
    /*for (i = 0; i < 4;) {

    }*/
    for (int i = 0; i < motor.leftMotors.size(); i++) { // Loops for each motor, pulls velocities from each
    total += motor.leftMotors[i].get_actual_velocity();
    }
    return total / motor.leftMotors.size();
     // Returns the left motor group encoder value (average speed
                // of the motor set)
    case 2:
    for (int i = 0; i < motor.rightMotors.size(); i++) {
    total += motor.rightMotors[i].get_actual_velocity();
    }
    return total / motor.rightMotors.size();
      // Returns the right motor group encoder value (average speed
                // of the motor set)
    case 3:
    for (int i = 0; i < motor.flywheelMotors.size(); i++) {
    total += motor.flywheelMotors[i].get_actual_velocity();
    }
    return total / motor.flywheelMotors.size();
             2; // Returns the flywheel motor group encoder value (average speed
                // of the motor set)
    default:
      return 0;
    }
  }
  float getPosition(int motorSet) {
        double total;
        switch (motorSet) {
    case 1:
    //std::vector<double> x = mtrgroup[0].get_actual_velocities();
    /*for (i = 0; i < 4;) {

    }*/
    for (int i = 0; i < motor.leftMotors.size(); i++) { // Loops for each motor, pulls velocities from each
    total += motor.leftMotors[i].get_position();
    }
    return total / motor.leftMotors.size();
     // Returns the left motor group encoder value (average speed
                // of the motor set)
    case 2:
    for (int i = 0; i < motor.rightMotors.size(); i++) {
    total += motor.rightMotors[i].get_position();
    }
    return total / motor.rightMotors.size();
      // Returns the right motor group encoder value (average speed
                // of the motor set)
    case 3:
    for (int i = 0; i < motor.flywheelMotors.size(); i++) {
    total += motor.flywheelMotors[i].get_position();
    }
    return total / motor.flywheelMotors.size();
             2; // Returns the flywheel motor group encoder value (average speed
                // of the motor set)
    default:
      return 0;
    }
  }
  float getFastVelocity(int motorSet) {
    /*pros::Motor L1(8, true); // Motor L1: Normal

    pros::Motor R1(20, false); // Motor R1: Reversed

    pros::Motor F1(3, false);
    */switch (motorSet) {
    case 1:
      return motor.leftMotors[0].get_actual_velocity(); // Returns the left motor group encoder value
      // (speed of first motor in the motor set)
    case 2:
      return motor.rightMotors[0].get_actual_velocity(); // Returns the right motor group encoder value
      // (speed of first motor in the motor set)
    case 3:
      return motor.flywheelMotors[0].get_actual_velocity(); // Returns the flywhhel motor group encoder value
      // (speed of first motor in the motor set)
    default:
      return 0;
    }
  }
};