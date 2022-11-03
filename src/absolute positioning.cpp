#include "include.cpp"
/*
This will include the code for absolute positioning, found here
https://wiki.purduesigbots.com/software/odometry
*/
const float ld = 0; // Left tracking wheel distance
const float rd = 0; // Right tracking wheel distance
const float bd = 0; // Back/Rear tracking wheel distance

float gp = 0;  // Previous global position vector
float go = 0;  // Previous global orientation vector
float gor = 0; // Global orientation at last reset
float deltaL = 0;
float deltaR = 0;
float thetaOne = 0;
float deltaTheta = 0;

float odometry() {

  thetaOne =
      go + (deltaL - deltaR) / (ld + rd); // Outputs radian value for deltaTheta
  deltaTheta = thetaOne - go;

  return 0.0;
}