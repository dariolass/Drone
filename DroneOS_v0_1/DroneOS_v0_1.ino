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
