#include "motors.cpp"
static float kP = 2;
static float kI = .04;
static float kD = .01;

int setSpeed(int desiredVelocity) {
    // desiredVelocity is in RPM
    float error = flywheel.get_actual_velocity() - desiredVelocity;
    float prevError;
    float prevIntegral;
    float integral = prevIntegral + error;
    prevIntegral = integral;
    float derivative = error - prevError;
    prevError = error;
    int v = ((kP * error) + (kD * derivative) + (kI * integral)) * 127;

    return v;
}