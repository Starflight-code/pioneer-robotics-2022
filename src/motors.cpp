#include "pros/motors.hpp"
#include "include.cpp"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"
#include <vector>
#include <string>
#include "variables.cpp"

// CONTAINS A LOT OF DEPRECATED CODE - Will be removed once it's tested

class Piston { // Piston class, supports ADI Pistons and includes tracking/setting
  
  private:
  std::vector<pros::ADIDigitalOut> piston;
  bool piston_state; // Allows for toggle/state fetching
  public:
  void init(int ADI_Port) { // Initializes the piston, this needs to be completed before it can be used
  piston[0] = pros::ADIDigitalOut(ADI_Port);
  piston[0].set_value(false); // Retracts the piston, syncing it's state
  piston_state = false; // Sets the piston state, allowing for tracking
  }
  void set(bool state) {
    piston[0].set_value(state); // Sets the piston value to whatever was supplied
    piston_state = state; // Updates the tracker
  }
  bool get() {
    return piston_state; // Returns the piston_state tracker value
  }};
class MotorGroup {
private:
  int current_limiter = 100;
  double current_speed;
  std::vector<pros::Motor> motors = {};

public:
/*

*/
  void init(std::vector<int> motor_ports, bool alternating,
            bool initial_reverse_state) 
    /* This is a initializer that generates an internal vector array 
    of pros::Motor(s) Call this when instanciating the class object. */
    { 
    // Ex MotorGroup leftMotors(left_ports, true, false); std::vector<int>
    // motor_ports should be a vector containing the motor numbers in order,
    // this is especially important if you are alternating them bool alternating
    // is a boolean value that tells our initializer to change every other motor
    // to a reversed one bool initial_reverse_state tells our initializer what
    // the first (or all values for non-alternating) reverse state will be. For
    // alternating, the next reverse state will always be opposite of this one.
    if (alternating) {
      bool t =
          initial_reverse_state; // Reverse state is used, as the contructor
                                 // doesn't like us adjusting given variables
      for (int i = 0; i < motor_ports.size();
           i++) { // Goes through all the ports and sets them accordingly
        motors.push_back(pros::Motor(motor_ports[i],
                                     t)); // Initializes an array of motors,
                                          // given the ports and reversal states
        t = !t; // Changes its state each time the loop occurs
      }
    } else { // If we are not supposed to alternate motor states, this runs
      for (int i = 0; i < motor_ports.size(); i++) {
        motors.push_back(pros::Motor(
            motor_ports[i],
            initial_reverse_state)); // Sets all motors to their ports and sets
                                     // the motor reverse state to
                                     // initial_reverse_state
      }
    }
  }
  void set(int speed) {
    current_speed = speed; // Sets the speed tracker
    // Sets the speed of all motors within vector<pros::Motor> motors to speed,
    // taking the limiter into account
    for (int i = 0; i < motors.size(); i++) {
      motors[i] = speed * (current_limiter / 100);
    }
  }
  int getSpeed() { return current_speed; }     // Fetches speed from tracker
  int getLimiter() { return current_limiter; } // Fetches limiter from variable
  void setLimiter(int limiter_setting) {
    current_limiter = limiter_setting;
  } // Sets the limiter variable
  double getPosition() {
    double total = 0;
    for (int i = 0; i < motors.size();
         i++) { // Loops for each motor, pulls velocities from each
      total += motors[i].get_position();
    }
    return total / motors.size();
  } // Returns the average of all positions

  double getVelocity() {
    double total = 0;
    for (int i = 0; i < motors.size();
         i++) { // Loops for each motor, pulls velocities from each
      total += motors[i].get_actual_velocity();
    }
    return total / motors.size();
  } // Returns the average of all velocities
  double getFastVelocity() {
    return motors[0]
        .get_actual_velocity(); // Returns the velocity of the first motor
  }
  double getFastPosition() {
    return motors[0].get_position(); // Returns the position of the first motor
  }
};
// class Motors{ DEPRECATED - Remove once above code is confirmed working

/*
    pros::Motor L1(9, true);  // Motor L1: Normal
    pros::Motor L2(5, false); // Motor L2: Reversed
    pros::Motor L3(4, true);  // Motor L3: Normal
    pros::Motor L4(8, false); // Motor L4: Reversed

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
    }*/
//};
class Motor_Class {
private:
  // std::vector<pros::Motor> leftMotors;
  // std::vector<pros::Motor> rightMotors;
  // std::vector<pros::Motor> flywheelMotors;

  MotorGroup leftMotors;
  MotorGroup rightMotors;
  MotorGroup flywheelMotors;
  MotorGroup spinnerMotors;
  Piston launcher;
  public:
  robot Robot;

  Motor_Class() {
    Robot.init('a'); // Sets up preset for robot 'a' for Artie and 'c' for Chance
    
    /*std::vector<int> leftPorts = {16, 6, 3, DEPRECATED - REMOVE ONCE PRESETS ARE TESTED
                                  8}; // Ports of left motors, from L1 to L4
    std::vector<int> rightPorts = {20, 19, 18,
                                   17}; // Ports of right motors, from R1 to R4
    std::vector<int> flywheelPorts = {1,
        4}; // Ports of flywheel motors, from F1 to F2
    std::vector<int> spinnerPorts = {13};*/
    leftMotors.init(Robot.leftPorts, Robot.leftAlt_Rev_States[0], Robot.leftAlt_Rev_States[1]);
    rightMotors.init(Robot.rightPorts, Robot.rightAlt_Rev_States[0], Robot.rightAlt_Rev_States[1]);
    flywheelMotors.init(Robot.flywheelPorts, Robot.flywheelAlt_Rev_States[0], Robot.flywheelAlt_Rev_States[1]);
    spinnerMotors.init(Robot.spinnerPorts,Robot.spinnerAlt_Rev_States[0],Robot.spinnerAlt_Rev_States[1]);
    leftMotors.setLimiter(Robot.limiter);
    rightMotors.setLimiter(Robot.limiter);
    flywheelMotors.setLimiter(Robot.limiter);
    spinnerMotors.setLimiter(Robot.limiter);
    launcher.init(1); // Placeholder port given, replace once decided
  }

private:
  float modifier = 1; // This reduced the motor power by POWER * modifier,
                      // should be between 0 and 1 or it will return an error.
  int left_motors = 0;
  int right_motors = 0;
  int flywheels = 0;
  bool init = true;

public:
  void setSpeed(int motorSet, int speed) {
    switch (motorSet) {

    case 1: // motorSpeed(1, speed) to set left motor group's speed
      // L1 = speed * modifier;
      // L2 = speed * modifier;
      // L3 = speed * modifier;
      // L4 = speed * modifier;
      leftMotors.set(speed);
      break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
      // R1 = speed * modifier;
      // R2 = speed * modifier;
      // R3 = speed * modifier;
      // R4 = speed * modifier;
      rightMotors.set(speed);
      break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
      // F1 = speed;
      // F2 = speed;
      flywheelMotors.set(speed);
      break;
    case 4: // motorSpeed(3, speed) to set spinner motor group's speed
      // F1 = speed;
      // F2 = speed;
      spinnerMotors.set(speed);
      break;
    default:
      return;
    }
    return;
  };

  int getSpeed(int motorSet) {
    switch (motorSet) {
    case 1:
      return leftMotors.getSpeed(); // Returns the left motor group tracker
                                    // value (speed of the motor set)
    case 2:
      return rightMotors.getSpeed(); // Returns the right motor group tracker
                                     // value (speed of the motor set)
    case 3:
      return flywheelMotors
          .getSpeed(); // Returns the flywheel motor group tracker value (speed
                       // of the motor set)
    case 4:
      return spinnerMotors
          .getSpeed(); // Returns the spinner motor group tracker value (speed
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
    switch (motorSet) {
    case 1:
      // std::vector<double> x = mtrgroup[0].get_actual_velocities();
      /*for (i = 0; i < 4;) {

      }*/
      return leftMotors.getVelocity();
      // Returns the left motor group encoder value (average speed
      // of the motor set)
    case 2:
      return rightMotors.getVelocity();
      // Returns the right motor group encoder value (average speed
      // of the motor set)
    case 3:
      return flywheelMotors
          .getVelocity(); // Returns the flywheel motor group encoder value
                          // (average speed of the motor set)
      case 4:
      return spinnerMotors
          .getVelocity(); // Returns the spinner motor group encoder value
                          // (average speed of the motor set)
    default:
      return 0;
    }
  }
  float getPosition(int motorSet) {
    double total;
    switch (motorSet) {
    case 1:
      // std::vector<double> x = mtrgroup[0].get_actual_velocities();
      /*for (i = 0; i < 4;) {

      }*/
      return leftMotors.getPosition();
      // Returns the left motor group encoder value (average speed
      // of the motor set)
    case 2:
      return rightMotors.getPosition();
      // Returns the right motor group encoder value (average speed
      // of the motor set)
    case 3:
      return flywheelMotors.getPosition();
      // Returns the flywheel motor group encoder value (average speed
      // of the motor set)
    case 4:
      return spinnerMotors
          .getPosition(); // Returns the flywheel motor group encoder value
                          // (average speed of the motor set)
    default:
      return 0;
    }
  }
  float getFastVelocity(int motorSet) {
    /*pros::Motor L1(8, true); // Motor L1: Normal

    pros::Motor R1(20, false); // Motor R1: Reversed

    pros::Motor F1(3, false);
    */
    switch (motorSet) {
    case 1:
      return leftMotors
          .getFastVelocity(); // Returns the left motor group encoder value
      // (speed of first motor in the motor set)
    case 2:
      return rightMotors
          .getFastVelocity(); // Returns the right motor group encoder value
      // (speed of first motor in the motor set)
    case 3:
      return flywheelMotors
          .getFastVelocity(); // Returns the flywhhel motor group encoder value
      // (speed of first motor in the motor set)
    default:
      return 0;
    }
  }
  float getFastPosititon(int motorSet) {
    /*pros::Motor L1(8, true); // Motor L1: Normal

    pros::Motor R1(20, false); // Motor R1: Reversed

    pros::Motor F1(3, false);
    */
    switch (motorSet) {
    case 1:
      return leftMotors
          .getFastPosition(); // Returns the left motor group encoder value
      // (speed of first motor in the motor set)
    case 2:
      return rightMotors
          .getFastPosition(); // Returns the right motor group encoder value
      // (speed of first motor in the motor set)
    case 3:
      return flywheelMotors
          .getFastPosition(); // Returns the flywhhel motor group encoder value
      // (speed of first motor in the motor set)
    default:
      return 0;
    }
  }
};