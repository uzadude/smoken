#include <data.h>
#include <HardwareSerial.h>

#define NUM_HIST_POINTS    30

dataMeasures measures;

void updateDQ(std::deque<double> &dq, double dv) {
  dq.push_back(dv);
  if (dq.size()>NUM_HIST_POINTS) {
    dq.pop_front();
  }

//   int dsize2 = dq.size();
//   for(int i = 0; i < dsize2; i++){
//       Serial.printf("%3.0f ", dq.at(i));
//   }
//   Serial.println("");
}

void addReads(double tmp1, double tmp2, double tmp3, double pwm, double rpm) {
    updateDQ(measures.tmpsQ1, tmp1);
    updateDQ(measures.tmpsQ2, tmp2);
    updateDQ(measures.tmpsQ3, tmp3);
    updateDQ(measures.pwm, pwm);
    updateDQ(measures.rpm, rpm);
    }

dataMeasures getMeasures() {
    return measures;
}

