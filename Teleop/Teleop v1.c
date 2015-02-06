#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S3,     irSeeker,       sensorPotentiometer) Wheres "HiTechnic1200 ir seeker"?
#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  mtr_S1_C1_1,     frontLeft,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     frontRight,    tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     backLeft,      tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     backRight,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     liftLeftTop,   tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     liftLeftBottom, tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     liftRightTop,  tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     liftRightBottom, tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    latchLeft,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    latchRight,           tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    intakeLeft,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_4,    intakeRight,          tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
 
/* December 19th, 2014 (Friday no-school!)
Just added lift and the joystick-if-statement
*/
 
#include "JoystickDriver.c"
 
int threshold=10;
int joyy1;
int joyx2;
int mode = 1;
 
task main()
{
 
  while(true)
  {
    getJoystickSettings(joystick);
 
    //Checking this first can save a little memory I guess
    if(abs(joystick.joy1_y1) > threshold || abs(joystick.joy1_x2) > threshold)
    {
      //joyy1 is either 100||-100 (if the joystick is past the limit (100)), 0 (if the joystick is under the threshold),
      //or the value of the joystick
    joyy1 = abs(joystick.joy1_y1) > 100 ? 100 * sgn(joystick.joy1_y1) : (abs(joystick.joy1_y1) < threshold ? 0 : joystick.joy1_y1);
    joyx2 = abs(joystick.joy1_x2) > 100 ? 100  * sgn(joystick.joy1_x2): (abs(joystick.joy1_x2) < threshold ? 0 : joystick.joy1_x2);
 
      motor[frontLeft] = (joyy1 + joyx2) / mode;
      motor[frontRight] = (joyy1 - joyx2) / mode;
      motor[backLeft] = (joyy1 + joyx2) / mode;
      motor[backRight] = (joyy1 - joyx2) / mode;
    }
 
    if(joy1Btn(4)==1)
    {
      if(mode==1)
      {
        mode = 4;
        } else {
        mode = 1;
      }
      wait1Msec(250);
    }
 
    if(joy1Btn(6)==1)
    {
      motor[liftLeftTop] = 100 / mode;
      motor[liftLeftBottom] = 100 / mode;
      motor[liftRightTop] = 100 / mode;
      motor[liftRightBottom] = 100 / mode;
      } else if(joy1Btn(5)==1) {
      motor[liftLeftTop] = -100 / mode;
      motor[liftLeftBottom] = -100 / mode;
      motor[liftRightTop] = -100 / mode;
      motor[liftRightBottom] = -100 / mode;
      } else {
      motor[liftLeftTop] = 0;
      motor[liftLeftBottom] = 0;
      motor[liftRightTop] = 0;
      motor[liftRightBottom] = 0;
    }
 
    if(joy1Btn(7)==1)
    {
      servo[latchLeft] = 180;
      servo[latchRight] = 180;
      } else if(joy1Btn(8)==1) {
      servo[latchLeft] = 0;
      servo[latchRight] = 0;
    }
 
    if(joy1Btn(4)==1)
    {
      servo[intakeLeft] = 200;
      servo[intakeRight] = 200;
      } else {
      servo[intakeLeft] = 50;
      servo[intakeRight] = 50;
    }
  }
}