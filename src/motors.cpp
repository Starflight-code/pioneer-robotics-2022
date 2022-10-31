#include "include.cpp"
// Limits the motor speed, must be 0 <= modifier <= 1
float modifier = 1;

// Imports the controller and maps it to variable master
//pros::Controller master(pros::E_CONTROLLER_MASTER);

// Imports the motors and maps them to variables
pros::Motor front_left(2,false); // Motor: Normal
pros::Motor front_right(1,true); // Motor: Reversed
pros::Motor rear_left(12,false); // Motor: Normal
pros::Motor rear_right(11,true); // Motor: Reversed
/* Add when the 8 motor drive is set up
pros::Motor front_left_2(x,true); // Motor: Reversed
pros::Motor front_right_2(x,false); // Motor: Normal
pros::Motor rear_left_2(x,true); // Motor: Reversed
pros::Motor rear_right_2(x,false); // Motor: Normal
*/
pros::Motor flywheel(3, false);
pros::Motor flywheel_2(4, true);
void motorSpeed(int motorSet, int speed) {
switch (motorSet) {
    case 1: // motorSpeed(1, speed) to set left motor group's speed
        front_left = speed * modifier;
        rear_left = speed * modifier;
        /*
        front_left_2 = speed * modifier;
        rear_left_2 = speed * modifier;
        */
        break;
    case 2: // motorSpeed(2, speed) to set the right motor group's speed
        front_right = speed * modifier;
        rear_right = speed * modifier;
        /*
        front_right_2 = speed * modifier;
        rear_right_2 = speed * modifier;
        */
        break;
    case 3: // motorSpeed(3, speed) to set flywheel motor group's speed
        flywheel = speed;
        flywheel = speed;
        break;
    default:
        //throw "Error, invalid input sent to function motorSpeed()";
        return;
        //break;
}}