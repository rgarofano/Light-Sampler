#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "A2D.h"
#include "sample_t.h"

#define NO_DATA -1
#define PREVIOUS_SAMPLE_WEIGHT 0.999

static int dips;
static long long lastSecondTimestamp = NO_DATA;

float Stats_calcAverageVoltage(sample_t* buffer, const int BUFF_SIZE)
{
    dips = 0;
    int currentSampleReading = buffer[0].voltageReading;
    float average = A2D_voltageFromReading(currentSampleReading);
    float dipThreshold = average - 0.1;
    float endDipThreshold = dipThreshold + 0.03;
    bool insideDip = false;

    for (int i = 1; i < BUFF_SIZE; i++) {
        currentSampleReading = buffer[i].voltageReading;
        float currentVoltage = A2D_voltageFromReading(currentSampleReading);
        average = (1 - PREVIOUS_SAMPLE_WEIGHT) * currentVoltage 
                  + PREVIOUS_SAMPLE_WEIGHT * average;

        if (!insideDip && currentVoltage <= dipThreshold) {
            dips++;
            insideDip = true;
        } else if (insideDip && currentVoltage >= endDipThreshold) {
            insideDip = false;
        }
    }

    return average;
}

float Stats_findMaxVoltage(sample_t* buffer, const int BUFF_SIZE)
{
    int max = INT_MIN;

    for (int i = 0; i < BUFF_SIZE; i++) {
        int voltageReading = buffer[i].voltageReading;
        if (voltageReading > max) {
            max = voltageReading;
        }
    }

    return A2D_voltageFromReading(max);
}

float Stats_findMinVoltage(sample_t* buffer, const int BUFF_SIZE)
{
    int min = INT_MAX;

    for (int i = 0; i < BUFF_SIZE; i++) {
        int voltageReading = buffer[i].voltageReading;
        if (voltageReading < min) {
            min = voltageReading;
        }
    }

    return A2D_voltageFromReading(min);
}

int Stats_getNumDips(sample_t* buffer, const int BUFF_SIZE)
{
    return dips;
}

static float nanoToMilliseconds(long long timeDifference)
{
    return (float) timeDifference / 1000000;
}

float Stats_calcAverageTimeIntreval(sample_t* buffer, const int BUFF_SIZE)
{
    long long sum = buffer[0].timestampNanoSec - lastSecondTimestamp;
    long long previousTime = buffer[0].timestampNanoSec;

    for (int i = 1; i < BUFF_SIZE; i++) {
        long long timestampNs = buffer[i].timestampNanoSec;
        long long interval = timestampNs - previousTime;
        sum += interval;
        previousTime = timestampNs;
    }

    return nanoToMilliseconds(sum / BUFF_SIZE);
}

float Stats_findMaxTimeIntreval(sample_t* buffer, const int BUFF_SIZE)
{
    long long max = buffer[0].timestampNanoSec - lastSecondTimestamp;
    long long previousTime = buffer[0].timestampNanoSec;

    for (int i = 1; i < BUFF_SIZE; i++) {
        long long timestampNs = buffer[i].timestampNanoSec;
        long long interval = timestampNs - previousTime;
        if (interval > max) {
            max = interval;
        }
        previousTime = timestampNs;
    }

    return nanoToMilliseconds(max);
}

float Stats_findMinTimeIntreval(sample_t* buffer, const int BUFF_SIZE)
{
    if (lastSecondTimestamp == NO_DATA) {
        lastSecondTimestamp = buffer[0].timestampNanoSec;
    }

    long long min = buffer[0].timestampNanoSec - lastSecondTimestamp;
    long long previousTime = buffer[0].timestampNanoSec;

    for (int i = 1; i < BUFF_SIZE; i++) {
        long long timestampNs = buffer[i].timestampNanoSec;
        long long interval = timestampNs - previousTime;
        if (interval < min) {
            min = interval;
        }
        previousTime = timestampNs;
    }

    lastSecondTimestamp = buffer[BUFF_SIZE - 1].timestampNanoSec;

    return nanoToMilliseconds(min);
}