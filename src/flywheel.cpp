#include "include.cpp"
#include "motors.cpp"
Motors motors;
static float kP = .2;
static float kI = .04;
static float kD = .01;
float error;
float prevError;
float prevIntegral;
float derivative;
float integral;

int setSpeed(int desiredVelocity) {
  // desiredVelocity is in RPM
  error = flywheel.get_actual_velocity() - desiredVelocity;
  integral = prevIntegral + error;
  prevIntegral = integral;
  derivative = error - prevError;
  prevError = error;

  return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
}