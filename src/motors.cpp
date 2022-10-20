#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <array>
#include <exception>
#include <string>
#include <tuple>
float modifier = 1;

pros::Motor front_left(2,false); // Motor: Normal
pros::Motor front_right(1,true); // Motor: Reversed
pros::Motor rear_left(12,false); // Motor: Normal
pros::Motor rear_right(11,true); // Motor: Reversed
/*
pros::Motor front_left_2(x,true); // Motor: Reversed
pros::Motor front_right_2(x,false); // Motor: Normal
pros::Motor rear_left_2(x,true); // Motor: Reversed
pros::Motor rear_right_2(x,false); // Motor: Normal
*/
pros::Motor flywheel(3);
pros::Motor flywheel_2(4);
void motorSpeed(int motorSet, int speed) {
switch (motorSet) {
    case 1:
        front_left = speed * modifier;
        rear_left = speed * modifier;
        break;
    case 2:
        front_right = speed * modifier;
        rear_right = speed * modifier;
        break;
    case 3:
        flywheel = speed;
        flywheel = speed;
        break;
    default:
        throw "Error, invalid input sent to function motorSpeed()";
        break;
}
}