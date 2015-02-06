/* Janurary 5th, 2015
Chris: Happy first Monday of the year! They are finishing up the robot, so I'll just make this program in the mean time
This encoder teleop is basically a measurer for the ecoder ticks we have to move. For example, instead of plugging in
random numbers until the robot goes where we want it to, we can go backwards: drive it where we want it to and get the
number we have to plug in
*/
 
#DEFINE BOUND(input, min, max) input > max ? max : (input < min ? min : input)
//Bound: takes the input and either returns max (if input is greater than max), min (if input is less than min), or input (if it fits in the range)
  int joy1y1 = 0;   //Instead of setting the motors to the joystick value, we set it to 2 variables so we can add the velocity together
  int joy1x2 = 0;
  nMotorEncoder[frontLeft] = 0;
  nMotorEncoder[frontRight] = 0;
  int leftEncoder = 0, rightEncoder = 0, lError = 0, rError = 0;
 
void runEncoders()
{
 nMotorEncoder[frontLeft] = 0;
  nMotorEncoder[frontRight] = 0;
   
  //lError = distance - nMotorEncoder[frontLeft];
  //rError = distance - nMotorEncoder[frontRight];
  //We just set the encoders to 0, so we don't have to subtract anything!
  motor[frontLeft] = motor[backLeft] = BOUND(leftEncoder * kp, -100, 100);
  motor[frontRight] = motor[backRight] = BOUND(rightEncoder * kp, -100, 100);
  while(nMotorRunState[frontLeft] != runStateIdle || nMotorRunState[frontRight] != runStateIdle)
  {
    if(nMotorEncoder[frontLeft] != nMotorEncoder[frontRight])   //If the encoders aren't equal (if one's faster)
    {
      error = nMotorEncoder[frontLeft] - nMotorEncoder[frontRight]; //Find out how far ahead/behind the left encoder is, then add it to the right motor (to fix the difference)
  lError = leftEncoder - nMotorEncoder[frontLeft]; //Adding the error to both won't fix the issue
  rError = rightEncoder - nMotorEncoder[frontRight] + error;
    } else {
  lError = leftEncoder - nMotorEncoder[frontLeft];
  rError = rightEncoder - nMotorEncoder[frontRight];
    }
  if(abs(lError) <= 0 || abs(rError) <= 0) 
  {
    motor[frontLeft] = motor[backLeft] = motor[frontRight] = motor[backRight] = 0;
  } else {
  motor[frontLeft] = motor[backLeft] = BOUND(lError * kp, -100, 100);
  motor[frontRight] = motor[backRight] = BOUND(rError * kp, -100, 100); 
  }
  }
}
 
task main()
{
 
  while(true)
  {
   getJoystickSettings(joystick);
     
    if(abs(joystick.joy1_y1) > threshold)
    {
     //joy1y1 = Bound(joystick.joy1_y1, -100, 100); 
      joy1y1 = joystick.joy1_y1; //We probably don't have to limit the joystick value because we're limiting it when we set the motors
    } else {
     joy1y1 = 0; 
    }
    if(abs(joystick.joy1_x2) > threshold)
    {
     //joy1x2 = Bound(joystick.joy1_x2, -100, 100); 
      joy1x2 = joystick.joy1_x2;  
    } else {
     joy1x2 = 0; 
    }
     
    /*
    Logic:
    ASSUME joy1y1 and joy1x2 are both positive. You want all the motors to go forward & right (it makes sense on joystick when the values are positive), so imagine which value(s) you need to negate. Then just take the individual equations and add the velocities together. When the values are negative, the equation should then make it go another direction correctly!
     
    Go forward:
    motor[frontLeft] = joy1y1;
    motor[frontRight] = joy1y1;
    motor[backLeft] = joy1y1;
    motor[backRight] = joy1y1;
     
    Turn right (the direction it goes when joy1x2 is positive):
    motor[frontLeft] = joy1x2;
    motor[frontRight] = -joy1x2;
    motor[backLeft] = joy1x2;
    motor[backRight] = -joy1x2;
     
    Front-left: (motor[frontLeft] = joy1y1 & motor[frontLeft] = joy1x2)
    Front-right: (motor[frontRight] = joy1y1 & motor[frontRight] = -joy1x2)
    Back-left: (motor[backLeft] = joy1y1 & motor[backLeft] = joy1x2)
    Back-right: (motor[backRight] = joy1y1 & motor[backRight] = -joy1x2)
     
    Then we Bound() it so we don't set it too high of a number (maybe it might mess up the encoder. Idk)
    */
    motor[frontLeft] = BOUND(joy1y1 + joy1x2, -100, 100);
    motor[frontRight] = BOUND(joy1y1 - joy1x2, -100, 100);
    motor[backLeft] = BOUND(joy1y1 + joy1x2, -100, 100);
    motor[backRight] = BOUND(joy1y1 - joy1x2, -100, 100);
     
    if(joy1Btn(5)==1) //Button to reset the encoders
    {
     nMotorEncoder[frontLeft] = 0;
      nMotorEncoder[frontRight] = 0;
    }
     
    if(joy1Btn(6)==1) //Button to save the encoders to run
    {
     leftEncoder = nMotorEncoder[frontLeft];
      rightEncoder = nMotorEncoder[frontRight];
    }
     
    if(joy1Btn(7)==1) //Button to run the saved encoders
    {
     if(leftEncoder != 0 && rightEncoder != 0)
     {
      runEncoders(); 
     }
    }
  }
}
