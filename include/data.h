#include <deque>

struct dataMeasures {
    std::deque<unsigned long> time_millis;
    std::deque<double> tmpsQ1;
    std::deque<double> tmpsQ2;
    std::deque<double> tmpsQ3;
    std::deque<double> pwm;
    std::deque<double> rpm;
};

void addReads(double tmp1, double tmp2, double tmp3, double pwm, double rpm);
dataMeasures getMeasures();
