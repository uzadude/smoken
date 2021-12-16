#include <PID_v1.h>

// PID
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);


void initPID(double setpoint) {
  Setpoint = setpoint;
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
}

void updatePID(double val) {
    Input = val;
    myPID.Compute();
}

double getPIDoutput() {
    return Output;
}