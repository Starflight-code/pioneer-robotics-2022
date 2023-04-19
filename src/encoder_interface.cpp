#include "pros/motors.hpp"
#include <vector>
#ifndef main_h_
#include "main.h"
#define main_h_
#endif

#ifndef motors_cpp_
#include "motors.cpp"
#define motors_cpp_
#endif

class encoderModule {
private:
    // Encoder modifiers that will be applied, gearboxes change the distance (rotationary) that the motor travels for a given encoder increase.
    // This change must of course be accounted for, which is the purpouse for these modifers.
    enum gearBox {
        blue = (2),
        green = (4),
        red = (5)
    };
    gearBox currentGearBox;
    MotorGroup interface;

    encoderModule(gearBox value, MotorGroup interface) {
        currentGearBox = value;
        this->interface = interface;
    }
    /**
     * @param totalRotation | degrees of total rotation desired
     * @return N/A
     */
    void rotate(int totalRotation, int atSpeed) {
        double compareValue = totalRotation * currentGearBox;
        interface.tarePosition();
        interface.set(atSpeed);
        while(interface.getFastPosition() < compareValue) { // 300 encoder units in 360 degrees (1.2 degrees per encoder unit)
            pros::delay(10);
        }
        interface.set(0); // Stops the motor after execution
    };
};