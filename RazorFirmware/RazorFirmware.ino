#include <Wire.h>

unsigned char readI2C(char deviceAddress, char registerAddress);
void writeI2C(char deviceAddress, char registerAddress, char data);

void setup()
{
  Serial.begin(57600);
  Wire.begin();

  //Sensor setup, read WHO-AM-I
  char configByte = 25;
  writeI2C(0x68, 0x16, 25);
  writeI2C(0x68, 0x15, 4);
}

void loop()
{
  int gyroX = 0;
  gyroX = readI2C(0x68, 0x1D)<<8;
  gyroX |= readI2C(0x68, 0x1E);
  Serial.println(gyroX, DEC);

  delay(5);
}

unsigned char readI2C(char deviceAddress, char registerAddress)
{
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.endTransmission();

  Wire.beginTransmission(deviceAddress);
  Wire.requestFrom(deviceAddress, 1);

  unsigned char data = 0;
  if (Wire.available()) {
    data = Wire.read();
  }

  Wire.endTransmission();
  return data;
}

void writeI2C(char deviceAddress, char registerAddress, char data)
{
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}
