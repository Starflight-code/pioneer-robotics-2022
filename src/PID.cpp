#include "include.cpp"
#include "motors.cpp"

/// Class for PID and other control algorithms
class Control_Algorithms {
private:                  // TODO: Tune constants as PID will have a decreased/reverse effect with incorrect constants
    const float kP = .2;  // Potential Constant
    const float kI = .04; // Integral Constant
    const float kD = .01; // Derivative Constant
    float error;
    float prevError;
    float prevIntegral;
    float derivative;
    float integral;
    Motor_Class Motors;

public:
    /// WIP code for PID
    int PID(int distance) {
        // desiredVelocity is in distance (unit - unknown, not sure it matters)
        error = distance - Motors.leftMotors.getFastPosition();
        integral = prevIntegral + error;
        prevIntegral = integral;
        derivative = error - prevError;
        prevError = error;

        return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
    }
    /// WIP code for PID
    int PID_Velocity(int desiredVelocity) {

        // desiredVelocity is in RPM
        error = Motors.flywheelMotors.getVelocity() - desiredVelocity;
        integral = prevIntegral + error;
        prevIntegral = integral;
        derivative = error - prevError;
        prevError = error;

        return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
    }
};