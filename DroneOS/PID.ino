double pitchPIDSetpoint, pitchPIDInput, pitchPIDOutput;
PID pitchPID(&pitchPIDInput, &pitchPIDOutput, &pitchPIDSetpoint, 1.7, 0.2, 0.6, DIRECT);

double rollPIDSetpoint, rollPIDInput, rollPIDOutput;
PID rollPID(&rollPIDInput, &rollPIDOutput, &rollPIDSetpoint, 1.7, 0.2, 0.6, DIRECT);

void initializePID()
{
  //Setup PID
  pitchPIDInput = 0;
  pitchPIDSetpoint = 0;
  pitchPID.SetMode(AUTOMATIC);
  pitchPID.SetOutputLimits(-500.0, 500.0);
  pitchPID.SetSampleTime(5);

  rollPIDInput = 0;
  rollPIDSetpoint = 0;
  rollPID.SetMode(AUTOMATIC);
  rollPID.SetOutputLimits(-500.0, 500.0);
  rollPID.SetSampleTime(5);
}
void computePID() 
{
  pitchPIDInput = pitchAngle;
  pitchPID.Compute();
  pitchPIDResult = pitchPIDOutput;

  rollPIDInput = rollAngle;
  rollPID.Compute();
  rollPIDResult = rollPIDOutput;
}

