#ifndef include_cpp_
#include "include.cpp"
#define include_cpp_
#endif

#ifndef motors_cpp_
#define motors_cpp_
#include "motors.cpp"
#endif

/// Class for PID and other control algorithms
class Control_Algorithms {
private:            // TODO: Tune constants as PID will have a decreased/reverse effect with incorrect constants
    float kP = .2;  // Potential Constant
    float kI = .04; // Integral Constant
    float kD = .01; // Derivative Constant
    float error;
    float prevError;
    float prevIntegral;
    float derivative;
    float integral;

public:
    Control_Algorithms(float Potential_Constant, float Integral_Constant, float Derivative_Constant) {
        kP = Potential_Constant;
        kI = Integral_Constant;
        kD = Derivative_Constant;
    }
    /// WIP code for PID
    /** Distance PID
     *
     */
    int PID_Distance(int distance, double motorPosition, bool zeroPower) {
        // desiredVelocity is in distance (unit - unknown, not sure it matters)
        error = distance - motorPosition;
        // integral = prevIntegral + error;
        // prevIntegral = integral;
        integral += error;
        if(error <= 0 && zeroPower) {
            integral = 0;
            prevIntegral = 0;
        }
        derivative = error - prevError;
        prevError = error;

        return ((kP * error) + (kD * derivative) + (kI * integral)) * 127;
    }
    /// WIP code for PID
    int PID_Velocity(int desiredVelocity, double currentVelocity, bool zeroPower) {

        // desiredVelocity is in RPM
        error = desiredVelocity - currentVelocity;
        integral += error;
        // integral = prevIntegral + error;
        // prevIntegral = integral;
        derivative = error - prevError;
        prevError = error;

        return ((kP * error) + (kD * derivative) + (kI * integral));
    }
    int PD_Velocity(double desiredVelocity, double currentVelocity) {

        // desiredVelocity is in RPM
        error = desiredVelocity - currentVelocity;
        derivative = error - prevError;
        prevError = error;

        return ((kP * error) + (kD * derivative)) /* * 127*/;
    }
};