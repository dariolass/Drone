/* This is DroneOS beta version 0.1!
 * Created by Dario Lass on 1-NOV-2015
 */

#include <Servo.h>
#include <PID_v1.h>

boolean readyMessageSent = false;
boolean razorInSync = false;

//--- FLIGHT  CONTROL VARs ----------
int pitch;        // rotation along axis with rotors 1 & 3
int roll;         // rotation along axis with rotors 2 & 4
int yaw;          //central rotation around core
int throttle;     //throttle

//---- MATH VARs --------------------
#define BASE_NUMBER_MIN 0
#define BASE_NUMBER_MAX 2000

#define MICROS_NUMBER_MIN 1100
#define MICROS_NUMBER_MAX 1900

//---- SENSOR VARs ------------------
double pitchAngle = 0;
double rollAngle = 0;
double yawAngle = 0;

//---- PIN DECLARATION --------------
#define MOTOR_PIN_1 3
#define MOTOR_PIN_2 5
#define MOTOR_PIN_3 6
#define MOTOR_PIN_4 9

//---- MOTOR OBJECT DECLARATION ----

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

//---- FUNCTION DECLARATION ---------
void    readRazor();
void    computePID();
void    initializeRazor();
boolean readToken(String token);

//Define Variables we'll be connecting to
double PIDSetpoint, PIDInput, PIDOutput;

PID myPID(&PIDInput, &PIDOutput, &PIDSetpoint, 5.0, 5.0, 1.0, DIRECT);

float pitchPIDResult  = 0;
float rollPIDResult   = 0;

// -------------------------- IMPLEMENTATION --------------------------

void setup() // ----- S E T U P
{
  Serial.begin(9600);
  Serial1.begin(9600);  //Wireless Communication serial port
  Serial2.begin(57600); //Sensor Communication serial port

  pitch = 0;
  roll = 0;
  yaw = 0;
  throttle = 0;

  motor1.attach(MOTOR_PIN_1);
  motor2.attach(MOTOR_PIN_2);
  motor3.attach(MOTOR_PIN_3);
  motor4.attach(MOTOR_PIN_4);

  delay(2000);  // 2 seconds should be enough

  //FIX: move this to its own setup function
  Serial1.write("COMM READY\n");
  
  //this should block the setup loop until the sync token is received
  initializeRazor();

  //Setup PID
  PIDInput = 0;
  PIDSetpoint = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-500.0, 500.0);
  myPID.SetSampleTime(10);
}

void loop() // ----- L O O P
{
  while (Serial1.available() > 0) { // reading WIRELESS SERIAL comm
    int integer1 = Serial1.parseInt(); 
    int integer2 = Serial1.parseInt(); 
    int integer3 = Serial1.parseInt(); 
    int integer4 = Serial1.parseInt();
    if (Serial1.read() == '\n') {
      pitch = integer1;
      roll = integer2;
      yaw = integer3;
      throttle = integer4;
    }
  }

  readRazor();

  // computePID();
}

void initializeRazor()
{
  // Set Razor output parameters
  Serial2.write("#ob");  // Turn on binary output
  Serial2.write("#o1");  // Turn on continuous streaming output
  Serial2.write("#oe0"); // Disable error message output
  
  // Clear input buffer up to here
  int bufferSize = Serial2.available();
  if (bufferSize > 0) {
    byte dump[bufferSize];
    Serial2.readBytes(dump, bufferSize);
  }
  
  Serial2.write("#s00");  // Request synch token

  while (!razorInSync) {
    razorInSync = readToken("#SYNCH00\r\n");
  }
  
  if (razorInSync) Serial.print("Razor In Sync!");
}

boolean readToken(String token) 
{
  // Wait until enough bytes are available
  if (Serial2.available() < token.length())
    return false;
  
  // Check if incoming bytes match token
  for (int i = 0; i < token.length(); i++) {
    if (Serial2.read() != token.charAt(i))
      return false;
  }
  
  return true;
}

void readRazor() 
{
  if (Serial2.available() >= 12) {
    byte yawData[4];
    yawData[0] = Serial2.read();
    yawData[1] = Serial2.read();
    yawData[2] = Serial2.read();
    yawData[3] = Serial2.read();
    yawAngle =    *((float*)(yawData));
    byte pitchData[4];
    pitchData[0] = Serial2.read();
    pitchData[1] = Serial2.read();
    pitchData[2] = Serial2.read();
    pitchData[3] = Serial2.read();
    pitchAngle =  *((float*)(pitchData));
    byte rollData[4];
    rollData[0] = Serial2.read();
    rollData[1] = Serial2.read();
    rollData[2] = Serial2.read();
    rollData[3] = Serial2.read();
    rollAngle =   *((float*)(rollData));
  }
}

void computePID() 
{
  //PID calculation for ONE axis
  Serial.print("Computing PID");
  //Pitch PID
  PIDInput = pitchAngle;
  PIDSetpoint = 1;
  Serial.print(PIDInput);
  Serial.print('\t');
  myPID.Compute();
  Serial.println(PIDOutput);
  pitchPIDResult = PIDOutput;
}

