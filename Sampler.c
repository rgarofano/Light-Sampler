#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include "Sleep.h"

#define MAX_NUM_SAMPLES 1024
#define PHOTORESISTOR_AIN 1
#define STORE_SAMPLE_DELAY_MS 1
#define ANALYZE_SAMPLES_DELAY_MS 1000

typedef struct {
    int voltage;
    long long timestampNanoSec;
} sample_t;

static sample_t buffer[MAX_NUM_SAMPLES];
static int numSamples = 0;
static bool shutdown = false;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static long long getTimeInNs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    return spec.tv_nsec;
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

static int calculateAverageVoltage()
{
    return 0;
}

static int findMaxVoltage()
{
    return 0;
}

static int findMinVoltage()
{
    return 0;
}

static int calculateNumDips()
{
    return 0;
}

static long long calculateAverageTimeIntreval()
{
    return 0;
}

static long long findMaxTimeIntreval()
{
    return 0;
}

static long long findMinTimeIntreval()
{
    return 0;
}

void* storeSamples(void* arg)
{
    while(!shutdown) {
        char* photoresistorReading = A2D_readVoltage(PHOTORESISTOR_AIN);
        int voltage = atoi(photoresistorReading);
        sample_t sample = {voltage, getTimeInNs()};
        addSample(sample);
        Sleep_waitForMs(STORE_SAMPLE_DELAY_MS);
    }
    pthread_exit(NULL);
}

void* analyzeSamples(void* arg)
{
    while(!shutdown) {
        Sleep_waitForMs(ANALYZE_SAMPLES_DELAY_MS);
        pthread_mutex_lock(&mutex);
        {
            int avgVoltage = calculateAverage();
            int maxVoltage = findMaxVoltage();
            int minVoltage = findMinVoltage();
            int numDips = calculateNumDips();
            long long avgTimeIntreval = calculateAverageTimeIntreval();
            long long maxTimeIntreval = findMaxTimeIntreval();
            long long minTimeIntreval = findMinTimeIntreval();
        }
        pthread_mutex_unlock(&mutex);
    }
}

void Sampler_startSampling(void)
{
    pthread_t storeSamplesTid;
    pthread_create(&storeSamplesTid, NULL, storeSamples, NULL);

    pthread_t analyzeSamplesTid;
    //TODO create analyzer thread
}

void Sampler_stopSampling(void)
{
    shutdown = true;
    pthread_mutex_unlock(&mutex);
}