#include "JoystickDriver.c"
 
  //DATE: 12/16/14
   
  // BUTTONS: -1 (none), 0 (grey), 1 (right), 2 (left), 3 (orange)
   
  // Encoder distance idea: drive it w/ joystick then print out the encoder value. Do it multiple times to get an average, then test it in autonomous
   
string locations[] = {"Platform", "Parking", "BottomOfPlatform", "60cm", "IRBeacon"};
int waitTimes[] = {0, 0, 0, 0, 0};  //Wait times between actions (to try to prevent crashing into others' autonomous)
int orderOfActions[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Stores the order of what functions to run ******************* change depending on how many actions there are
/*
1 = get to bottom of ramp
2 = get the 60cm thing
3 = take the 60cm to the parking zone
4 = find beacon & knock down kickstand
*/
short currLocation = 0;
bool colorIsBlue = true;
 
//Autonomous motion-tuning variables
int lError = 0;
int rError = 0;
int motorError = 0;
int kp = 2;
//const float WHEEL_DIAMETER_INCHES = 3;
//const float MOVEMENT_FACTOR = (1440/(WHEEL_DIAMETER_CM * PI));
/*
Description of above:
The circumference of the wheel is equal to the distance traveled in 1 rotation (which is diameter * PI)
 
1440 ticks in 1 rotation -> 1440 ticks per 9.42477796077inches(circumference of wheel) -> divide -> 152.788745368 ticks per inch
*/
const float ENCODER_TICKS_PER_INCH = 152.788745368;
const int perfect90 = 1980; // [16.5 inches * pi (circle that a point turn makes) / 4 (90 degrees is 360 degrees divided by 4)] * ENCODER_TICKS_PER_INCH
 
#define BOUND(num, low, high) (num < low) ? low : ((num > high) ? high : num)   //We don't want to set the motors higher than -100 or 100, so limit it!
 
/*
Updates the display during selection screen
*/
void updateDisplay(short onArrayIndex)
{
  if(onArrayIndex <= 7)  //run this section if the #index fits the screen (which is from lines 0 to 7)
  {
  if(orderOfActions[onArrayIndex]==1)
          {
           nxtDisplayString(onArrayIndex, "Forward"); 
          } else if(orderOfActions[onArrayIndex]==2){
            nxtDisplayString(onArrayIndex, "Go to 60cm");
          } else if(orderOfActions[onArrayIndex]==3){
           nxtDisplayString(onArrayIndex, "Take to Parking"); 
          } else if(orderOfActions[onArrayIndex]==4){
           nxtDisplayString(onArrayIndex, "Knock Kickstand"); 
          }
  } else if(onArrayIndex > 7) {  //run this section if the #index would be off the screen (8 or higher)
    if(orderOfActions[onArrayIndex]==1)
          {
           nxtDisplayString(onArrayIndex - 7, "Forward"); 
          } else if(orderOfActions[onArrayIndex]==2){
            nxtDisplayString(onArrayIndex - 7, "Go to 60cm");
          } else if(orderOfActions[onArrayIndex]==3){
           nxtDisplayString(onArrayIndex - 7, "Take to Parking"); 
          } else if(orderOfActions[onArrayIndex]==4){
           nxtDisplayString(onArrayIndex - 7, "Knock Kickstand"); 
          }
  }
   
}
 
/*
Takes the distance to go (in inches) and speed then goes that far (with proportional PID!)
*/
void moveVertical(int distInInches, int speed = 100, bool seeking = false)
{ 
  int encoderDist = distInCm * ENCODER_TICKS_PER_INCH;  //Calculate how many encoder ticks to go from the given distance (inches)
  nMotorEncoder[frontLeft] = 0;
  nMotorEncoder[frontRight] = 0;
   
  //lError = distance - nMotorEncoder[frontLeft];
  //rError = distance - nMotorEncoder[frontRight];
  //We just set the encoders to 0, so both will just equal "distance"!
  motor[frontLeft] = motor[backLeft] = BOUND(encoderDist * kp, -speed, speed);
  motor[frontRight] = motor[backRight] = BOUND(encoderDist * kp, -speed, speed);
  while(nMotorRunState[frontLeft] != runStateIdle || nMotorRunState[frontRight] != runStateIdle)
  {
    //If sonar does not see anything, then go forward
    if(seeking && SensorValue[BEACON] == 5) //if the robot is in "seeking" mode and it finds the beacon
    {
       motor[frontLeft] = motor[backLeft] = motor[frontRight] = motor[backRight] = 0;
      return;
    }
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
  motor[frontLeft] = motor[backLeft] = BOUND(lError * kp, -speed, speed);
  motor[frontRight] = motor[backRight] = BOUND(rError * kp, -speed, speed); 
  }
  }
}
 
/*
Takes the encoder count and speed, then goes that far. Does not take distance because the person would have to calculate the distance anyways, and tuning it is easier probably...
*/
void turn(int distance, int speed = 50)
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
 
/*
Goes to the bottom of the ramp as a central point, depending on the current location
*/
void goToBottomOfRamp()
{
 switch(currLocation)
   case 0:  //Case: on the platform
    moveVertical(10, 50); //Go forward for 5000 units to go down ramp
  case 1:   //Case: in the parking zone
  turn(-50, 50);    //Turn right just a little to face bottom of ramp
  moveVertical(10, 50);
  case 2:   //Case: on the bottom of the ramp (do nothing!)
  break;
  case 3:   //Case: at the 60cm goal
  moveVertical(10, 50);
  break;
  case default:
  break;
  currLocation = 2; //Current location is "BottomOfRamp"
}
 
/*
Finds the IR Beacon and knocks down the kickstand
*/
void findIr()
{
  if(currLocation != 2) //If not at bottom of ramp
  {
    getToBottomOfRamp();
  }
  DISTANCE
  turn(perfect90, 50);  //Perfect left turn to face the goal
  moveVertical(20, 50); //Approach the circular ball holder
  turn(perfect90, 50);  //Perfect left turn to face parallel with the circular ball holder
  moveVertical(20, 50);
  turn(-perfect90, 50);
  moveVertical(5, 50);  //Now it should be at #BeaconPosition1
  for(int i = 0; i < 3; i++)
  {
    moveVertical(DISTANCE, 50, true);    //Go until it /should've/ seen the beacon. If it goes past this distance, it means it's not in that position
    if(SensorValue[BEACON]==5)  //Re-check if it's at 5, and if it is, then hit the kickstand (Which is to the right of the beacon)
    {
      moveVertical(-DISTANCE, 50);
      turn(-perfect90, 50);
      moveVertical(DISTANCE, 100);  //FULL POWER (b/c we gotta take that kickstand down)
      return;
    }
  }
   motor[frontLeft] = motor[backLeft] = motor[frontRight] = motor[backRight] = 0;
}
 
/*
Go to 60cm goal
*/
void getTo60Cm()
{
  if(currLocation != 2) //If not at bottom of ramp
  {
   goToBottomOfRamp(); 
  }
  moveVertical(10, 50);
  turn(perfect90 * 2, 50);  //Do 180 b/c the clamp is on the back of the robot
  moveVertical(-2, 50); //Back up a little to get on the thing
  if(colorIsBlue)
  {
   while(SensorValue[light] == BLUE)    //While it sees blue, move left and right (to find the hole to clamp onto)
   {
    turn(1, 50);
     turn(-1, 50);
   }
  } else {
   while(SensorValue[light] == RED)     //While it sees red, move left and right (to find the hold to clamp onto)
   {
     turn(1, 50);
     turn(-1, 50);
   }
  }
  motor[frontLeft] = motor[frontRight] = motor[backLeft] = motor[backRight] = 0;
  servo[CLAMP] = DOWN;
  currLocation = 3;
}
 
/*
Take the goal to the parking zone   (ASSUMES IT'S RAN RIGHT AFTER GetTo60Cm())
*/
void takeToParking()
{
 nMotorEncoder[frontLeft] = 0;
  while(nMotorEncoder[frontLeft] < DISTANCE)
  {
    motor[frontLeft] = motor[backLeft] = 50;
    motor[frontRight] = motor[backRight] = 25;
  }
  nMotorEncoder[frontRight] = 0;
  while(nMotorEncoder[frontRight] < DISTANCE)
  {
    motor[frontLeft] = motor[backLeft] = 0;
    motor[frontRight] = motor[backRight] = 50;
  }
  moveVertical(DISTANCE, 50);
  turn(-perfect90 * 2, 50); //Do a 180 to put the goal in the parking zone
  motor[frontLeft] = motor[frontRight] = motor[backLeft] = motor[backRight] = 0;
  servo[CLAMP] = UP;
}
 
void selectStartPoint()
{
  nxtDisplayString(0, "Start Position?");
  while(nNxtButtonPressed != 3)
  {
    if(nNxtButtonPressed == 1 || nNxtButtonPressed == 2)
    {
     if(currLocation == 0) currLocation = 1;
     else currLocation = 0;
     nxtDisplayString(1, "!%s",locations[currLocation]);
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
    if(nNxtButtonPressed == 0) 
    {
      selectInit(1);
      return;
    }
  }
  nxtDisplayString(3, ">"); //Change from "!Red" to ">Red"
}
 
void selectListOfAction()
{
  short onArrayIndex = 0;   //Stores which function the nxt is selecting at the moment. Start at 0
  while(nNxtButtonPressed != 3)
  {
    if(onArrayIndex % 2 == 0)
    {
     while(nNxtButtonPressed != 3)
       {
         if(nNxtButtonPressed == 1)
         {
          waitTimes[onArrayIndex/2]++;  //divide by 2 so indexes are (0 [0/2], 1 [2/2], 2 [4/2], 3 [6/2], 4 [8/2], & 5 [10/2])
           nxtDisplayString(onArrayIndex, "Wait %d seconds", waitTimes[onArrayIndex/2]);
         } else if(nNxtButtonPressed == 2) {
          waitTimes[onArrayIndex/2]--;
           nxtDisplayString(onArrayIndex, "Wait %d seconds", waitTimes[onArrayIndex/2]);
         }
         wait1Msec(500);
       }
      onArrayIndex++;
    } else {
        while(nNxtButtonPressed != 3)
        {
          orderOfActions[onArrayIndex] = 1;
          if(nNxtButtonPressed == 1)    //If you press right, then either set it to 1 (if it's 4) or add 1
          {
            if(orderOfActions[onArrayIndex] + 1 > 4)
            {
              orderOfActions[onArrayIndex] = 1;
            } else {
             orderOfActions[onArrayIndex]++; 
            }
          updateDisplay(onArrayIndex);
          } else if(nNxtButtonPressed == 2) //If you press left, then either set it to 4 (if it's 1) or subtract 1
          {
           if(orderOfActions[onArrayIndex] - 1 < 1)
           {
            orderOfActions[onArrayIndex] = 4;
           } else {
             orderOfActions[onArrayIndex]--;
           }
            updateDisplay(onArrayIndex);
          }
        }
      onArrayIndex++;
    }
  }
}
 
void selectInit()
{
 // switch(menu)
 // case 1:
  selectStartPoint(); //While loop to select Platform or Parking [short startLocation (index 0 or 1 of locations[])]
 // break;
 // case 2:
  selectStartColor(); //While loop to select Blue or Red [bool colorIsBlue]
 // break;
 // case 3:
  selectListOfAction(); //While loop to select what to do, step by step (IE, go down ramp, choose to wait, etc etc etc)
 // case default:
 // break;
}
 
void runActions()
{
 for(int i = 0; i < 10; i++)
 {
   if(i % 2 == 0)
   {
    wait1Msec(waitTimes[i/2]); 
   } else {
  switch(orderOfActions[i]) //Only does odds (1,3,5,...) because the if loop above takes care of evens
   case 1:
   goToBottomOfRamp();  //Get to bottom of ramp
   break;
   case 2:
   getTo60Cm(); //Get to the 60cm
   break;
     case 3:
     takeToParking();
     break;
     case 4:
     findIr();  //Finds the IR seeker and knocks down kickstand
     break;
     case default:
     break;
   }
 }
}
 
task main()
{
selectInit();
}
