#include "include.cpp"
#include "motors.cpp"

/// Class for PID and other control algorithms
class Control {
private: // TODO: Tune constants as PID will have a decreased/reverse effect with incorrect constants
  const float kP = .2; // Potential Constant
  const float kI = .04; // Integral Constant
  const float kD = .01; // Derivative Constant
  float error;
  float prevError;
  float prevIntegral;
  float derivative;
  float integral;
  Motor_Class Motors;

public:
  /*int PID(int distance) {
    // desiredVelocity is in RPM
    //error = Motors.getVelocity(3) - desiredVelocity; !! DEPRECATED Velocity PID
    //error = distance - Motors.getPosition(1);
    integral = prevIntegral + error;
    prevIntegral = integral;
    derivative = error - prevError;
    prevError = error;

    return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
  }
  int PID_Velocity(int desiredVelocity) {

    // desiredVelocity is in RPM
    //error = Motors.getVelocity(3) - desiredVelocity; 
    integral = prevIntegral + error;
    prevIntegral = integral;
    derivative = error - prevError;
    prevError = error;

    return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
  }*/
};