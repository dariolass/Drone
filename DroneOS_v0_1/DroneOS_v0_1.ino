/* This is DroneOS beta version 0.1!
 * Created by Dario Lass on 1-NOV-2015
 */

boolean readyMessageSent = false;
boolean messageToIMUSent = false;

//--- FLIGHT  CONTROL VARs ----------
int pitch;        // rotation along axis with rotors 1 & 3
int roll;         // rotation along axis with rotors 2 & 4
int yaw;          //central rotation around core
int throttle;     //overall thrust distributed to rotors

//---- MATH VARs --------------------
#define BASE_NUMBER_MIN 0
#define BASE_NUMBER_MAX 2000

#define MICROS_NUMBER_MIN 1110
#define MICROS_NUMBER_MAX 1900

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

  if (millis() >= 2000 && !messageToIMUSent) {
    Serial2.write('4');
    messageToIMUSent = true;
  }
  
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
  
  //'$accelx,accely,accelz,gyrox,gyroy,gyroz,magx,magy,magz#\n\r'

  while (Serial2.available() > 0) { // reading SENSOR SERIAL comm
    if (Serial2.read() == '$') {
      accPitch = Serial2.parseInt();
      accRoll = Serial2.parseInt();
      accYaw = Serial2.parseInt();
      gyroPitch = Serial2.parseInt();
      gyroRoll = Serial2.parseInt();
      gyroYaw = Serial2.parseInt();
      magPitch = Serial2.parseInt();
      magRoll = Serial2.parseInt();
      magYaw = Serial2.parseInt();
    }
  }

  //Serial.print("Motor 1: "); Serial.print(calculateThrustForRotor(1)); Serial.print("\tMotor 3: "); Serial.println(calculateThrustForRotor(3));

  motor1.writeMicroseconds(calculateThrustForRotor(1));
  motor2.writeMicroseconds(calculateThrustForRotor(2));
  motor3.writeMicroseconds(calculateThrustForRotor(3));
  motor4.writeMicroseconds(calculateThrustForRotor(4));
}

int calculateThrustForRotor(int motor) {
  int factor = 5;
  int baseNumber = 0;
  switch (motor) {
    case 1:
      baseNumber = throttle + roll - (factor*accPitch);
      break;
    case 2:
      baseNumber = throttle + pitch - (factor*accRoll);
      break;
    case 3:
      baseNumber = throttle + roll + (factor*accPitch);
      break;
    case 4:
      baseNumber = throttle + pitch + (factor*accRoll);
      break;
  }
  int microsToWrite = map(baseNumber, BASE_NUMBER_MIN, BASE_NUMBER_MAX, MICROS_NUMBER_MIN, MICROS_NUMBER_MAX);
  return microsToWrite;
}
