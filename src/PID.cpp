#include "include.cpp"
#include "motors.cpp"

class PID {
private:
  const float kP = .2; // Potential Constant
  const float kI = .04; // Integral Constant
  const float kD = .01; // Derivative Constant
  float error;
  float prevError;
  float prevIntegral;
  float derivative;
  float integral;

public:
  int full(int desiredVelocity) {
    Motor_Class Motors;
    // desiredVelocity is in RPM
    error = Motors.getVelocity(3) - desiredVelocity;
    integral = prevIntegral + error;
    prevIntegral = integral;
    derivative = error - prevError;
    prevError = error;

    return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
  };
};