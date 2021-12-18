#include <PID_v1.h>

// PID
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT);


void setPIDconfs(int setpoint, int min, int max, int Kp, int Ki, int Kd) {
  Setpoint = setpoint;
  myPID.SetOutputLimits(min, max);
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetMode(AUTOMATIC);
}

void updatePID(double val) {
    Input = val;
    myPID.Compute();
}

double getPIDoutput() {
    return Output;
}