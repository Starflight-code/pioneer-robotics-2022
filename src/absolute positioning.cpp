#include "main.h"
/*
This will include the code for absolute positioning, found here https://wiki.purduesigbots.com/software/odometry
*/
float ld = 0; // Left tracking wheel distance 
float rd = 0; // Right tracking wheel distance
float bd = 0; // Back/Rear tracking wheel distance
float gp = 0; // Previous global position vector
float go = 0; // Previous global orientation vector
float gor = 0; // Global orientation at last reset

