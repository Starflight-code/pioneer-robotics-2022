#include "main.h"
#include "pros/rtos.h"
#include "controls.cpp"
void controls_fn(void* param) {
    while(true) {
    control_listener();
    pros::c::delay(50);
    }
 }