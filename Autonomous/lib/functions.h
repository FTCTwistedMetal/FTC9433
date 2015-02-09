//nMotorEncoder[liftLeftTop] is for the frontLeft motor
//nMotorEncoder[liftRightBottom] is for the frontRight motor
//It's not plugged into the same motor controller because of convenience (their on opposite sides of the mob)
/*liftLeftTop is reversed so the encoder is in sync with the front left motor
	(IE, when it the motor goes backwards, the encoder will go backwards too)*/
void moveStraight(int distInInches)
{
	int turnFactor = 0;
	int encoderDist = distInInches * ENCODER_TICKS_PER_INCH;
	nMotorEncoder[liftLeftTop] = 0;
	nMotorEncoder[liftRightBottom] = 0;
	speed = BOUND((int)encoderDist * kpd, -75, 75);	//We don't need to calc distError b/c encoders are 0 rn
	motor[frontLeft] = speed;
	motor[frontRight] = speed;
	motor[backLeft] = speed;
	motor[backRight] = speed;
	while(nMotorRunState[frontLeft] != runStateIdle || nMotorRunState[frontRight] != runStateIdle)
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
		distError = encoderDist - (nMotorEncoder[liftLeftTop] + nMotorEncoder[liftRightBottom])/2;

		speed = BOUND((int)distError * kpd, -75, 75);	//Limit so we have some room for the turnFactor
		/*Since turnFactor is basically the difference between the left and right motors, we'll
		add it to the right motors and subtract it from the left motors. When inversed, it'll still work*/
		motor[frontLeft] = speed - turnFactor;
		motor[frontRight] = speed + turnFactor;
		motor[backLeft] = speed - turnFactor;
		motor[backRight] = speed + turnFactor;
	}
}

void turn(int turnFactor, int encoderDist)	//If turn factor is negative, turn left. If positive, turn right
{
	nMotorEncoder[liftLeftTop] = 0;
	nMotorEncoder[liftRightBottom] = 0;
	speed = BOUND((int)encoderDist * kpd, -50, 50);	//We don't need to calc distError b/c encoders are 0 rn
	motor[frontLeft] = speed * turnFactor;	//Not to be confused with the PID turnFactor in moveStraight()
	motor[frontRight] = -speed * turnFactor;
	motor[backLeft] = speed * turnFactor;
	motor[backRight] = -speed * turnFactor;
	while(nMotorRunState[frontLeft] != runStateIdle || nMotorRunState[frontRight] != runStateIdle)
	{
		//Subtract the two to find the average b/c they're going opposite directions
		distError = encoderDist - (nMotorEncoder[liftLeftTop] - nMotorEncoder[liftRightBottom])/2;
		
		speed = BOUND((int)distError * kpd, -50, 50);	//We only needed turnFactor to kickstart the direction
		motor[frontLeft] = speed;
		motor[frontRight] = -speed;
		motor[backLeft] = speed;
		motor[backRight] = -speed;
	}
}

//ASSUMES: the robot is parallel to the center goal (IR is facing position 1)
void trackForIR()
{
	int target = 5000;	//Adjust for how far to go forward before turning and starting to look for the next position
	for(int i = 0; i < 3; i++)
	{
		nMotorEncoder[liftLeftTop] = 0;
		nMotorEncoder[liftRightBottom] = 0;
		while(SensorValue[irSeeker] != 5 && (nMotorEncoder[liftLeftTop] + nMotorEncoder[liftRightBottom])/2 < target)
		{
			/** Copied and pasted from moveStraight(). We can't use that function because it would be too
			busy in the while loop in moveStraight() to check if the irSeeker is 5 yet**/
			if (abs(nMotorEncoder[liftLeftTop] - nMotorEncoder[liftRightBottom]) < controllable)
			{
				error = nMotorEncoder[liftLeftTop] - nMotorEncoder[liftRightBottom];
	
				integral += error;
				/**
				 *
				 * We can either "dampen" the integral term by multiplying it by 2/3, or we can set a limit
				 *
				 *  **/
				derivative = error - lastError;
				lastError = error;
				turnFactor = (int)((error * kp) + (integral * ki) + (derivative * kd));
			}
			else
			{
				turnFactor = 0;
			}
			distError = encoderDist - (nMotorEncoder[liftLeftTop] + nMotorEncoder[liftRightBottom])/2;
	
			speed = BOUND((int)distError * kpd, -75, 75);
			motor[frontLeft] = speed - turnFactor;
			motor[frontRight] = speed + turnFactor;
			motor[backLeft] = speed - turnFactor;
			motor[backRight] = speed + turnFactor;
		}
		if(SensorValue[irSeeker] == 5)	//The while loop could end without finding the IR, so check first.
		{
			//DO STUFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		}
		else
		{
			turn(1, 5000);	//ADJUST DISTANCE TO TURN!
		}
		/*If the irSeeker isn't 5 (if the while loop ended because of the encoder-check), then just start
		tracking the next position (turn the repeat)*/
	}
}
