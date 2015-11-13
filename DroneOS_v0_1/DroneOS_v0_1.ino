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
int throttle;     //overall thrust distributed to rotors

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
boolean checkForSyncMarker();
void    readRazor();
void    computePID();

boolean PIDModeAutomatic = false;
//Define Variables we'll be connecting to
double PIDSetpoint, PIDInput, PIDOutput;

PID myPID(&PIDInput, &PIDOutput, &PIDSetpoint, 5.0, 5.0, 1.0, DIRECT);

float pitchPIDResult  = 0;
float rollPIDResult   = 0;

// -------------------------- IMPLEMENTATION --------------------------

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //Wireless Communication serial port
  Serial2.begin(57600); //Sensor Communication serial port

  pitch = 0;
  roll = 0;
  yaw = 0;
  throttle = 0;

  motor1.attach(MOTOR_PIN_1);
  motor2.attach(MOTOR_PIN_2);
  motor3.attach(MOTOR_PIN_3);
  motor4.attach(MOTOR_PIN_4);

  delay(3000);  // 3 seconds should be enough
  
  // Set Razor output parameters
  Serial2.write("#ob");  // Turn on binary output
  Serial2.write("#o1");  // Turn on continuous streaming output
  Serial2.write("#oe0"); // Disable error message output
  
  // Synch with Razor
  //Serial2.clear();  // Clear input buffer up to here
  Serial2.write("#s00");  // Request synch token

  Serial1.write("COMM READY\n");

  //Setup PID
  PIDInput = 0;
  PIDSetpoint = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-500.0, 500.0);
  myPID.SetSampleTime(10);
}

void loop() {
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

  if (!razorInSync) {
    razorInSync = checkForSyncMarker();
  } else {
    readRazor();
    computePID();
  }
  
  int thrust2 = map(throttle+pitchPIDResult,0,2000,1100,1800);
  int thrust4 = map(throttle-pitchPIDResult,0,2000,1100,1800);
  Serial.print(thrust2);
  Serial.print("\t");
  Serial.println(thrust4);
  //motor1.writeMicroseconds();
 // motor2.writeMicroseconds(thrust2);
  //motor3.writeMicroseconds();
  //motor4.writeMicroseconds(thrust4);
}

boolean checkForSyncMarker() {
  String marker = "#SYNCH00\r\n";
  if (Serial2.available() < marker.length()) 
    return false;
  for (int i = 0; i < marker.length(); i++) {
  if (Serial2.read() != marker.charAt(i))
    return false;
  }
  Serial.println("Razor is in sync!");
  return true;
}

void readRazor() {
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

void computePID() {
  //Pitch PID
  PIDInput = pitchAngle;
  PIDSetpoint = 1;
  Serial.print(PIDInput);
  Serial.print('\t');
  myPID.Compute();
  Serial.println(PIDOutput);
  pitchPIDResult = PIDOutput;
  
  
//  //Roll PID
//  PIDInput = rollAngle;
//  PIDSetpoint = 0;
//  myPID.Compute();
//  rollPIDResult = PIDOutput;
}

