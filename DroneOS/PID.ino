double PIDSetpoint, PIDInput, PIDOutput;
PID myPID(&PIDInput, &PIDOutput, &PIDSetpoint, 5.0, 5.0, 1.0, DIRECT);

void initializePID()
{
  //Setup PID
  PIDInput = 0;
  PIDSetpoint = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-500.0, 500.0);
  myPID.SetSampleTime(10);
}

void computePID() 
{
  //PID calculation for ONE axis
  //Pitch PID
  PIDInput = pitchAngle;
  PIDSetpoint = 1;
  Serial.print(PIDInput);
  Serial.print('\t');
  myPID.Compute();
  Serial.println(PIDOutput);
  pitchPIDResult = PIDOutput;
}

