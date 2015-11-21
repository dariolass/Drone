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

float pitchPIDResult  = 0;
float rollPIDResult   = 0;

//---- MATH VARs --------------------
#define BASE_NUMBER_MIN 0
#define BASE_NUMBER_MAX 2000

#define MICROS_NUMBER_MIN 1000
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
void    initializeRazor();
void    initializeWireless();
void    initializePID();

void    readRazor();
void    readWireless();
void    computePID();

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

  delay(2000);  // 2 seconds idle time for sensors and comm to get ready
  
  //initialize IMU, wireless comm, and PID
  initializeRazor();
  initializeWireless();
  initializePID();
}

void loop() // ----- L O O P
{
  
  readRazor();
  readWireless();
  computePID();

  int motor2Thrust = throttle + pitchPIDResult;
  motor2Thrust = map(motor2Thrust, BASE_NUMBER_MIN, BASE_NUMBER_MAX, MICROS_NUMBER_MIN, MICROS_NUMBER_MAX);
  motor2Thrust = constrain(motor2Thrust, MICROS_NUMBER_MIN, MICROS_NUMBER_MAX);
  motor2.writeMicroseconds(motor2Thrust);
  
  int motor4Thrust = throttle - pitchPIDResult;
  motor4Thrust = map(motor4Thrust, BASE_NUMBER_MIN, BASE_NUMBER_MAX, MICROS_NUMBER_MIN, MICROS_NUMBER_MAX);
  motor4Thrust = constrain(motor4Thrust, MICROS_NUMBER_MIN, MICROS_NUMBER_MAX);
  motor4.writeMicroseconds(motor4Thrust);

  motor1.writeMicroseconds(MICROS_NUMBER_MIN);
  motor3.writeMicroseconds(MICROS_NUMBER_MIN);
  
  Serial.print(motor2Thrust);
  Serial.print('\t');
  Serial.println(motor4Thrust);
}
