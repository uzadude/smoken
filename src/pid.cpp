#include <PID_v1.h>

// PID
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, 1, 0, 0, DIRECT);


void setPIDconfs(int setpoint, int min, int max, float Kp, float Ki, float Kd) {
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