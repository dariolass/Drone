/* This is DroneOS beta version 0.1!
 * Created by Dario Lass on 1-NOV-2015
 */

boolean readyMessageSent = false;

//--- FLIGHT  CONTROL VARs ----------
int pitch;        // rotation along axis with rotors 1 & 3
int roll;         // rotation along axis with rotors 2 & 4
int yaw;          //central rotation around core
int throttle;     //overall thrust distributed to rotors

//---- MATH VARs --------------------
#define baseNumber 1000
#define stabilisationSensitivity 1 //CHANGE !

//---- SENSOR VARs ------------------
int gyroPitch;
int gyroRoll;
int gyroYaw;
int accPitch;
int accRoll;
int accYaw;
int magPitch;
int magRoll;
int magYaw;

//---- PIN DECLARATION --------------
#define MOTOR_PIN_1 3
#define MOTOR_PIN_2 5
#define MOTOR_PIN_3 6
#define MOTOR_PIN_4 9

//---- MOTOR OBJECT DECLARATION -----
#include <Servo.h>

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

// -------------------------- IMPLEMENTATION --------------------------

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //Wireless  Communication serial port
  Serial2.begin(9600); //Sensor    Communication serial port

  pitch = 0;
  roll = 0;
  yaw = 0;
  throttle = 0;

  gyroPitch = 0;
  gyroRoll = 0;
  gyroYaw = 0;
  accPitch = 0;
  accRoll = 0;
  accYaw = 0;
  magPitch = 0;
  magRoll = 0;
  magYaw = 0;
  
  motor1.attach(MOTOR_PIN_1);
  motor2.attach(MOTOR_PIN_2);
  motor3.attach(MOTOR_PIN_3);
  motor4.attach(MOTOR_PIN_4);
}

void loop() {
  //Telling the controller side that communication is ready after 1 s
  if (millis() >= 1000 && !readyMessageSent) {
    Serial1.write("COMM READY\n");
    readyMessageSent = true;
  }
  while (Serial1.available() > 0) {
    int integer1 = Serial1.parseInt(); int integer2 = Serial1.parseInt(); int integer3 = Serial1.parseInt(); int integer4 = Serial1.parseInt();
    if (Serial1.read() == '\n') {
      pitch = integer1;
      roll = integer2;
      yaw = integer3;
      throttle = integer4;
    }
  }
}

void serialEvent2(){
  //read SENSOR comm buffer
}

int calculateThrustForRotor(int motor) {
  if (motor == 1 || 3) {
    // affected by ROLL rotation
  } else {
    // affected by PITCH rotation
  }
}

