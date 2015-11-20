void initializeWireless()
{
  Serial1.write("COMM READY\n");
}

void readWireless()
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
}

