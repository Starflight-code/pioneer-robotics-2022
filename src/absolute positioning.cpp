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
float deltaTheta = 0;
float x = 0;
float y = 0;
float x_trans = 0;
float y_trans = 0;
class odometry {
private:
  const float ld = 0; // Left tracking wheel distance
  const float rd = 0; // Right tracking wheel distance
  const float bd = 0; // Back/Rear tracking wheel distance
public:
  float x;
  float y;
  float thetaFinal = 0;
  float gp = 0;  // Previous global position vector
  float go = 0;  // Previous global orientation vector
  float gor = 0; // Global orientation at last reset
  float deltaL = 0;
  float deltaR = 0;
  float deltaLr = 0; // deltaL at last reset
  float deltaRr = 0; // deltaR at last reset
  float deltaD = 0;  // change in distance
  float deltaDr = 0; // change in distance at last reset
  float deltaS = 0;
  float thetaOne = 0;
  float deltaTheta = 0;
  float thetaM = 0;
  float odo() {
    thetaFinal = gor + (deltaLr - deltaRr) /
                           (ld + rd); // Outputs radian value for deltaTheta
    deltaTheta = thetaFinal - go;
    if (-0.01 < deltaTheta and deltaTheta < 0.01) {
      x_trans = deltaS;
      y_trans = deltaR;
    } else {
      x_trans = 2 * sin(deltaTheta / 2) * ((deltaS / deltaTheta) + bd);
      y_trans = 2 * sin(deltaTheta / 2) * ((deltaR / deltaTheta) + rd);
    }
    thetaM = thetaFinal + deltaTheta / 2;
    deltaD = deltaD - thetaM;
    deltaDr = deltaD;
    go = gor + deltaD;
    x += x_trans;
    y += y_trans;
    return 0.0;
  }
};