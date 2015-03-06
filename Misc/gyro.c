//twistedmetal131@gmail.com Orlando 1 to 5

//Here's basically how I would use a gyro sensor for going straight
/*NOTE: a gyro does NOT tell you what direction it is. It only tells you the angular velocity (Mrs. Sims didn't really
teach us this), which is basically the rate at which it is turnning (E.g "30 degrees per second")*/

/*A structure is basically putting all these variables under 1 name. You'd use "name.turnRate" to use the turnRate.
If you kind of get Object Oriented Programming, then understand that you can instantiate this structure (dw if you don't get it)*/
typedef struct
{
  tSensors sensorID;  //ID of the sensor (like... S3 for port 3)
  int turnRate; //The turn rate (the value the sensor gives)
  int deadBand; //Basically like the threshold we use for joysticks. We don't care if the turn rate is like... "0.1"
  int offSet; //The offset. Hopefully you get what it means. Typically around 598, but different for each, so we needa calibrate
  float heading;  //The current heading (direction/degrees. IE heading of 180 means it's facing the other way)
} GYRO;

int prevTime, currTime; //Keeps track of current/previous time so we know the delta time (change in time)

GYRO gyro;  //Make the "gyro" variable

void initGyro(tSensors id) //Initialize the gyro sensor (give it default values)
{
  gyro.sensorID = id;
  gyro.turnRate = 0;
  gyro.heading = 0;
  gyro.deadBand = 2;  //You can change this if you want the threshold to be bigger/smaller
  gyro.offSet = 0;  //We'll calibrate it in "calibrateGyro"
}

void calibrateGyro()
{
  int turnRate;
  int numSample = 50; //Number of samples to take. More = more accurate (but you can only be so accurate anyways...)
  
  for(int i = 0; i < numSample; i++)  //Do it 50 times
  {
    turnRate = SensorValue[gyro.sensorID];  //Get value of gyro
    gyro.offSet += turnRate;  //Add it all up so we can divide it and get the average
    wait1Msec(10);  //Wait a bit (1/100 of a second)
  }
  gyro.offSet /= numSample; //Divide the sum by how many numbers to get the average (ends up usually around 598)
}

task gyroTask() //A task is basically a function, except it runs at the SAME TIME as "task main()"
{
  while(true)
  {
    currTime = nPgmTime;  //Current time is the current time that the program has be running (use it as a checkpoint)
    if(currTime - prevTime >= 5)  //if it's been 5ms since the last time.
    {
      gyro.turnRate = SensorValue[gyro.sensorID]; //Get the value of the gyro
      gyro.turnRate -= gyro.offSet; //Take out the offset (or else it'll give you like.... 600)
      if(abs(gyro.turnRate) > gyro.deadBand)  //if we care about the value (if it's greater than deadBand (which is 2))
      {
        gyro.heading += (float) (gyro.turnRate * (currTime-prevTime)/1000);
        /*
        Basically we're taking the rate it's turning at (let's say 30 degrees a second, since the gyro measures
        the speed per second) and multiply it by how long it's been turning since we last checked (divide by 1000
        to get it in seconds).
        An example of this would be a car going at 90 miles per hour. If last time we checked was 1:25pm, then
        we check again and it's 1:30pm (it's been 5 minutes), then we multiply (90 * (5/60)) to get the distance
        */
      }
      prevTime = currTime;  //Update the previous time
    }
  }
}

task main()
{
initGyro(S1); //Put what port you're using the gyro on in there (IE port 3 would be "S3")
calibrateGyro();
StartTask(gyroTask);  //Start keeping track of the direction

/*
TODO: PID using this gyro sensor thingy
*/
}
