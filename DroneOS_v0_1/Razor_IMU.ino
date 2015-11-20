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
