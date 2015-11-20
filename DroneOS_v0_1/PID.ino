double PIDSetpoint, PIDInput, PIDOutput;
PID myPID(&PIDInput, &PIDOutput, &PIDSetpoint, 5.0, 5.0, 1.0, DIRECT);

float pitchPIDResult  = 0;
float rollPIDResult   = 0;

void computePID() 
{
  //PID calculation for ONE axis
  Serial.print("Computing PID");
  //Pitch PID
  PIDInput = pitchAngle;
  PIDSetpoint = 1;
  Serial.print(PIDInput);
  Serial.print('\t');
  myPID.Compute();
  Serial.println(PIDOutput);
  pitchPIDResult = PIDOutput;
}

