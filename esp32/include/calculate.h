#ifndef CALCULATE_H
#define CALCULATE_H
#include <Arduino.h>

extern float rmssd;
extern int beatAvg;

void calc_rmssd(uint32_t data);

#endif