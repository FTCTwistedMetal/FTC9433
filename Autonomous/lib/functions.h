//nMotorEncoder[liftLeftTop] is for the frontLeft motor
//nMotorEncoder[liftRightBottom] is for the frontRight motor
//It's not plugged into the same motor controller because of convenience (their on opposite sides of the mob)
/*liftLeftTop is reversed so the encoder is in sync with the front left motor
	(IE, when it the motor goes backwards, the encoder will go backwards too)*/
void moveStraight(int distInInches)
{
	int encoderDist = distInInches * ENCODER_TICKS_PER_INCH;
	nMotorEncoder[liftLeftTop] = 0;
	nMotorEncoder[liftRightBottom] = 0;
	while (nMotorRunState[frontLeft] != runStateIdle || nMotorRunState[frontRight] != runStateIdle)
	{
		//Only do PID if the motors are within the controllable range
		if (abs(nMotorEncoder[liftLeftTop] - nMotorEncoder[liftRightBottom]) < controllable)
		{
			//Error is the difference between the encoders
			error = nMotorEncoder[liftLeftTop] - nMotorEncoder[liftRightBottom];

			integral += error;	//Add error to integral
			/**
			 *
			 * We can either "dampen" the integral term by multiplying it by 2/3, or we can set a limit
			 *
			 *  **/
			derivative = error - lastError;	//Find the slope (rate of change)
			lastError = error;	//Update lastError for the next run-thru

			//Add the P.I.D terms to determine how much to turn the robot to get it to go straight
			turnFactor = (int)((error * kp) + (integral * ki) + (derivative * kd));
		}
		else
		{
			turnFactor = 0;
		}
		//Error of distance is the (set point - average of encoders)
		distError = encoderDist - (nMotorEncoder[liftLeftTop] + nMotorEncoder[liftRightBottom])/2

		speed = BOUND((int)distError * kpd, -75, 75);	//Limit so we have some room for the turnFactor
		//Since turnFactor is basically the difference between the left and right motors, we'll
		//add it to the right motors and subtract it from the left motors. When inversed, it'll still work
		motor[frontLeft] = speed - turnFactor;
		motor[frontRight] = speed + turnFactor;
		motor[backLeft] = speed - turnFactor;
		motor[backRight] = speed + turnFactor;
	}
}
