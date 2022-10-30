/*** Used to calculate all statistics of interest for the 
     Sampler module                                       ***/

#ifndef STATS_H
#define STATS_H

#include "sample_t.h"

float Stats_calcAverageVoltage(sample_t* buffer, const int BUFF_SIZE);
float Stats_findMaxVoltage(sample_t* buffer, const int BUFF_SIZE);
float Stats_findMinVoltage(sample_t* buffer, const int BUFF_SIZE);
int Stats_getNumDips(sample_t* buffer, const int BUFF_SIZE);

float Stats_calcAverageTimeIntreval(sample_t* buffer, const int BUFF_SIZE);
float Stats_findMaxTimeIntreval(sample_t* buffer, const int BUFF_SIZE);
float Stats_findMinTimeIntreval(sample_t* buffer, const int BUFF_SIZE);

#endif