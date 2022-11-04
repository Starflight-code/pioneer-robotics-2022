#include "include.cpp"
#include "motors.cpp"
Motors motors;


class Flywheels {
  private:
    const float kP = .2;
    const float kI = .04;
    const float kD = .01;
    float error;
    float prevError;
    float prevIntegral;
    float derivative;
    float integral;
  public:
    int calcSpeed(int desiredVelocity) {
    // desiredVelocity is in RPM
    error = flywheel.get_actual_velocity() - desiredVelocity;
    integral = prevIntegral + error;
    prevIntegral = integral;
    derivative = error - prevError;
    prevError = error;

    return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
    };
};