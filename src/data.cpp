#include <data.h>
#include <HardwareSerial.h>

#define NUM_HIST_POINTS    100
#define HIST_SAMPLE_INTERVAL    5000
unsigned long updateDataMillis = millis();

dataMeasures measures;
int rowCount = 0;

void downsample() {
  if (rowCount>NUM_HIST_POINTS) {

    Serial.println("Down-sampling data");
    // skip the first entry, to preserve start time
    for (int i = 1; i< (rowCount / 2) ; i++){

        int newRow = (i * 2) - 1;

        measures.time_millis[i] = measures.time_millis[newRow];
        measures.tmpsQ1[i] = measures.tmpsQ1[newRow];
        measures.tmpsQ2[i] = measures.tmpsQ2[newRow];
        measures.tmpsQ3[i] = measures.tmpsQ3[newRow];
        measures.pwm[i] = measures.pwm[newRow];
        measures.rpm[i] = measures.rpm[newRow];
      }
      //reset rowCount to the new, smaller number so any new writes will start after this culling
      rowCount = (rowCount / 2);
      Serial.println(rowCount);
    }

}

void checkMem() {
  int ALLOCATED_RAM = 1000;
  uint32_t free=system_get_free_heap_size() - ALLOCATED_RAM;
  
  int numberOfRows = free / (sizeof(double)*5+ sizeof(unsigned long));
  Serial.print("max number of data rows: ");
  Serial.println(numberOfRows);
}

void initData() {
  checkMem();
  measures.time_millis = new unsigned long [NUM_HIST_POINTS];
  measures.tmpsQ1 = new double [NUM_HIST_POINTS];
  measures.tmpsQ2 = new double [NUM_HIST_POINTS];
  measures.tmpsQ3 = new double [NUM_HIST_POINTS];
  measures.pwm = new double [NUM_HIST_POINTS];
  measures.rpm = new double [NUM_HIST_POINTS];
}

void addReads(double tmp1, double tmp2, double tmp3, double pwm, double rpm) {
  if (millis() - updateDataMillis > HIST_SAMPLE_INTERVAL) {
    updateDataMillis = millis();
    measures.time_millis[rowCount] = updateDataMillis;
    measures.tmpsQ1[rowCount] = tmp1;
    measures.tmpsQ2[rowCount] = tmp2;
    measures.tmpsQ3[rowCount] = tmp3;
    measures.pwm[rowCount] = pwm;
    measures.rpm[rowCount] = rpm;
    rowCount++;
  }
  downsample();
  //checkMem();
}

dataMeasures getMeasures() {
    return measures;
}

int getDataRowCount() {
  return rowCount;
}
