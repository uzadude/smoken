#include <deque>

struct dataMeasures {
    unsigned long *time_millis;
    double *tmpsQ1;
    double *tmpsQ2;
    double *tmpsQ3;
    double *pwm;
    double *rpm;
};

void initData();
int getDataRowCount();
void addReads(double tmp1, double tmp2, double tmp3, double pwm, double rpm);
dataMeasures getMeasures();
