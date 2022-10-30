#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "Sleep.h"
#include "A2D.h"
#include "Stats.h"
#include "LEDMatrix.h"
#include "sample_t.h"

#define MAX_NUM_SAMPLES 1024
#define PHOTORESISTOR_AIN 1
#define STORE_SAMPLE_DELAY_MS 1
#define ANALYZE_SAMPLES_DELAY_MS 1000

static sample_t buffer[MAX_NUM_SAMPLES];
static int numSamples = 0;
static bool shutdown = false;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static long long getTimeInNs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long timestamp = seconds * 1000000000 + nanoSeconds;
    return timestamp;
}

static void addSample(sample_t sample)
{
    pthread_mutex_lock(&mutex);
    {
        if (!shutdown) {   
            buffer[numSamples++] = sample;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void* storeSamples(void* arg)
{
    long long startTime = getTimeInNs();

    while(!shutdown) {
        int photoresistorReading = A2D_readVoltage(PHOTORESISTOR_AIN);
        long long timestampNs = getTimeInNs() - startTime;
        sample_t sample = {photoresistorReading, timestampNs};
        addSample(sample);
        Sleep_waitForMs(STORE_SAMPLE_DELAY_MS);
    }
    pthread_exit(NULL);
}

static void clearBuffer()
{
    memset(buffer, 0, MAX_NUM_SAMPLES*sizeof(sample_t));
    numSamples = 0;
}

void* analyzeSamples(void* arg)
{
    int numDips;
    float maxVoltage;
    float minVoltage;
    float minTimeIntreval;
    float maxTimeIntreval;

    while(!shutdown) {
        Sleep_waitForMs(ANALYZE_SAMPLES_DELAY_MS);
        pthread_mutex_lock(&mutex);
        {
            if (!shutdown) {
                maxVoltage = Stats_findMaxVoltage(buffer, numSamples);
                minVoltage = Stats_findMinVoltage(buffer, numSamples);
                minTimeIntreval = Stats_findMinTimeIntreval(buffer, numSamples);
                maxTimeIntreval = Stats_findMaxTimeIntreval(buffer, numSamples);
                float avgVoltage = Stats_calcAverageVoltage(buffer, numSamples);
                float avgTimeIntreval = Stats_calcAverageTimeIntreval(buffer, numSamples);
                numDips = Stats_getNumDips(buffer, numSamples);
                printf("Intreval ms (%f, %f) avg = %f Samples V (%f, %f) avg = %f # Dips: %d Samples: %d\n"
                    , minTimeIntreval, maxTimeIntreval, avgTimeIntreval, minVoltage, maxVoltage, avgVoltage
                    ,  numDips, numSamples);
                clearBuffer();
            }
        }
        pthread_mutex_unlock(&mutex);

        LEDMatrix_updateDisplayValues(numDips, maxVoltage, minVoltage, minTimeIntreval, maxTimeIntreval);
    }
    pthread_exit(NULL);
}

void Sampler_startSampling(void)
{
    pthread_t storeSamplesTid;
    pthread_create(&storeSamplesTid, NULL, storeSamples, NULL);

    pthread_t analyzeSamplesTid;
    pthread_create(&analyzeSamplesTid, NULL, analyzeSamples, NULL);
}

void Sampler_stopSampling(void)
{
    shutdown = true;
    pthread_mutex_unlock(&mutex);
}