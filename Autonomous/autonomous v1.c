#include "JoystickDriver.c"
 
  //DATE: 12/10/14
   
string locations[] = {"Platform", "Parking", "BottomOfPlatform", "60cm"};
short currLocation = 0;
short startLocation = 0;
bool colorIsBlue = true;
 
//Autonomous motion-tuning variables
int lError = 0;
int rError = 0;
int motorError = 0;
int kp = 2;
//const float WHEEL_DIAMETER_CM = 7.5;
//const int WHEEL_DIAMETER_INCHES = ((float) WHEEL_DIAMETER_CM / 0.393701);
//const float MOVEMENT_FACTOR = (1440/(WHEEL_DIAMETER_CM * PI));
/*
Description of above:
The circumference of the wheel is equal to the distance traveled in 1 rotation (which is diameter * PI)
 
1440 ticks in 1 rotation -> 1440 ticks per 23.5619449019cm (circumference) -> divide -> 61.154981473 ticks per cm
*/
const float ENCODER_TICKS_PER_CM = 61.154981473;
 
const int perfect90 = 1440; //Tune to make a perfect 90 degree turn
#define BOUND(num, low, high) (num < low) ? low : ((num > high) ? high : num)
 
void moveVertical(int distInCm, int speed = 50)
{
  int encoderDist = distInCm * ENCODER_TICKS_PER_CM;
  nMotorEncoder[frontLeft] = 0;
  nMotorEncoder[frontRight] = 0;
   
  //lError = distance - nMotorEncoder[frontLeft];
  //rError = distance - nMotorEncoder[frontRight];
  //We just set the encoders to 0, so both will just equal "distance"!
  motor[frontLeft] = motor[backLeft] = BOUND(encoderDist * kp, -100, 100);
  motor[frontRight] = motor[backRight] = BOUND(encoderDist * kp, -100, 100);
  while(nMotorRunState[frontLeft] != runStateIdle || nMotorRunState[frontRight] != runStateIdle)
  {
    if(nMotorEncoder[frontLeft] != nMotorEncoder[frontRight])
    {
      error = nMotorEncoder[frontLeft] - nMotorEncoder[frontRight];
  lError = encoderDist - nMotorEncoder[frontLeft]; //Adding the error to both won't fix the issue
  rError = encoderDist - nMotorEncoder[frontRight] + error;
    } else {
  lError = encoderDist - nMotorEncoder[frontLeft];
  rError = encoderDist - nMotorEncoder[frontRight];
    }
  if(lError <= 0 || rError <= 0)
  {
    motor[frontLeft] = motor[backLeft] = motor[frontRight] = motor[backRight] = 0;
  } else {
  motor[frontLeft] = motor[backLeft] = BOUND(lError * kp, -100, 100);
  motor[frontRight] = motor[backRight] = BOUND(rError * kp, -100, 100); 
  }
  }
}
 
void turn(int distance = perfect90, int speed = 50)
{
  nMotorEncoder[frontLeft] = 0;
   
  while(abs(nMotorEncoder[frontLeft]) < distance)
  {
   motor[frontLeft] = -speed;
    motor[frontRight] = speed;
    motor[backLeft] = -speed;
    motor[backRight] = speed;
  }
  motor[frontLeft] = motor[frontRight] = motor[backLeft] = motor[backRight] = 0;
}
   
void goToBottomOfRamp()
{
 switch(currLocation)
   case 0:  //Case: on the platform
    moveVertical(5000, 50); //Go forward for 5000 units to go down ramp
  case default:
  break;
}
 
void selectStartPoint()
{
  nxtDisplayString(0, "Start Position?");
  while(nNxtButtonPressed != 3)
  {
    if(nNxtButtonPressed == 1 || nNxtButtonPressed == 2)
    {
     if(startLocation == 0) startLocation = 1;
     else startLocation = 0;
     nxtDisplayString(1, "!%s",locations[startLocation]);
    wait1Msec(250);
    }
    wait1Msec(250);
  }
  nxtDisplayString(1, ">"); //Change from "!Platform" to ">Platform"
}
 
void selectStartColor()
{
  nxtDisplayString(2, "Color?");
  while(nNxtButtonPressed != 3)
  {
    if(nNxtButtonPressed == 1 || nNxtButtonPressed == 2)
    {
      colorIsBlue = !colorIsBlue;
      if(colorIsBlue) nxtDisplayString(3, "!Blue");
      else nxtDisplayString(3, "!Red");
      wait1Msec(250);
    }
    wait1Msec(250);
  }
  nxtDisplayString(3, ">"); //Change from "!Red" to ">Red"
}
 
void selectInit(short menu)
{
  switch(menu)
    case 1:
  selectStartPoint(); //While loop to select Platform or Parking [short startLocation (index 0 or 1 of locations[])]
  break;
  case 2:
  selectStartColor(); //While loop to select Blue or Red [bool colorIsBlue]
  break;
  case default:
  break;
}
 
task main()
{
selectInit();
}
