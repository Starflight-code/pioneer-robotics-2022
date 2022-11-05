#include "include.cpp"
pros::Rotation left = pros::Rotation(12); //left rotation sensor on port 12, not reversed
pros::Rotation right = pros::Rotation(13); //right rotation sensor on port 13, not reversed
pros::Rotation back = pros::Rotation(14); //back rotation sensor on port 14, not reversed

const float sL = 0; //x distance between tracking center and left tracking wheel
const float sR = 0; //x distance between tracking center and right tracking wheel
const float sC = 0; //y distance between tracking center and back tracking wheel
const float wheelDiameter = 2.75; //diameter of tracking wheel
/*
CONVENTIONS
Angles are measured in radians
Distance is measured in inches
*/

float previousAngle = 0; //changes initial orientation of the robot
float globalPosition [2] = {0,0};//changes initial position of the robot(x,y)

float leftTranslation;
float rightTranslation;
float backTranslation;
float changeInAngle;
float globalAngle;
float arcRadius;
float localPosition [2];//(x,y)
float polarChangeInPosition [2];//(r,theta)
float globalChangeInPosition [2];//(x,y)



float  rotationToTranslation (float angle) {
  /*converts angle in centidegrees to angle in radians and uses arc length formula
    to calculate distance of wheel travel.*/
  float convertedAngle = (angle/100) * (3.14159/180);
  return (wheelDiameter/2) * convertedAngle;
}



void findPosition() {

  while(true) {
    leftTranslation = rotationToTranslation(left.get_position());
    rightTranslation = rotationToTranslation(right.get_position());
    backTranslation = rotationToTranslation(back.get_position());

    changeInAngle = (leftTranslation - rightTranslation) / (sL + sR);


    float arcRadiusLateral = (rightTranslation/ changeInAngle) + sR;
    float arcRadiusHeading = (backTranslation/changeInAngle) + sC;

    if(changeInAngle < .01 and changeInAngle > -.01) {
      localPosition [0] = backTranslation;
      localPosition [1] = rightTranslation;
    }
    else {
    localPosition [0] = 2 * sin(changeInAngle/2) * (arcRadiusHeading);
    localPosition [1] = 2 * sin(changeInAngle/2) * (arcRadiusLateral);
    }
    //now that we have local position, we must rotate this vector back to the global space.
    //we do this by first converting to polar coordinates

    polarChangeInPosition [0] = sqrt(pow((localPosition[0]),2) + pow((localPosition[1]),2));
    //finds radius for our polar local coordinates

    polarChangeInPosition [1] = atan2(localPosition[0],localPosition[1]);
    //finds theta for our polar local coordinates

    polarChangeInPosition [1] -= (previousAngle + (changeInAngle/2));
    //rotate back by -(theta not + (delta theta) / 2)

    //now to convert back to cartesian coordinates

    globalChangeInPosition [0] = polarChangeInPosition[0] * cos(polarChangeInPosition[1]);
    globalChangeInPosition [1] = polarChangeInPosition[0] * sin(polarChangeInPosition[1]);

    //finally, add globalChangeInPosition to globalPosition to receive our new global position

    globalPosition[0] += globalChangeInPosition[0];
    globalPosition[1] += globalChangeInPosition[1];

    //calculate global orientation
    globalAngle = previousAngle + changeInAngle;
    previousAngle = globalAngle;
  }
}
