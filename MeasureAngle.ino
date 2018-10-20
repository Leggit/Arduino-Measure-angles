//include library for I2C communication
#include <Wire.h>

//define some values that will remain constant 
#define MPU 0b1101000 //I2C address of the angle sensor
#define RAD_TO_DEG 57.296 //multiplier for converting from Radians ro Degrees
#define RATE_TO_RAD 0.000001066 //convert angular rate value returned by gyro to radians
#define RATE_TO_DEG 0.0000611 //convert angular rate value returned by gyro to degrees

//variables used in angle measurements 
float pitch, roll;//hold to final calculated angles
int gyroOffsetX, gyroOffsetY, gyroOffsetZ;//calibration values
long accelX, accelY, accelZ, accelVector;
int gyroX, gyroY, gyroZ, temp;

//used when accessing arrays that store both roll and pitch - 
#define PITCH 2 //pitch will always be in position 2
#define ROLL 1 //roll will always be in position 1

//change these if the angles have the wrong signs
#define PITCH_SIGN_CORRECTION 1
#define ROLL_SIGN_CORRECTION -1

//variables used to make sure each loop is 4 microseconds long
long timer;
long loopTime;
int loopCounter = 0;

boolean error = false;

//method prototypes
void setupMPU(void);
void calibrateGyro(void);
void setStartAngles(void);
void calculateAngles(void);
void printAngles(float,float);

void setup() {
  Serial.begin(115200);//debugging 

  //make sure there is a connection to the sensor
  do{
    error = testMPU();
    //Serial.println("Dodgy connection with the MPU");//if connected for debugging, the user will be alerted
    delay(1000);
  }while(error == true);

  //set up methods
  setupMPU();
  calibrateGyro();
  setStartAngles();
}

void loop() {
  timer = micros();//remember the start time of the loop
  loopCounter = loopCounter + 1;//incement loop counter

  calculateAngles();//calculate the current pitch and roll of the sensor

  //every 5 loopsprint the values
  if(loopCounter == 5){
    loopCounter = 0;
    printAngles(roll, pitch);//switch these if the display if all wrong
  }
  
  while (micros() - timer < 4000);//wait until 4 milliseconfs have happened
}
