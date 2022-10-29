#include <stdio.h>
#include <stdlib.h>
#include "File.h"

#define MAX_SIZE 1024
#define LOWEST_AIN_NUMBER 0
#define HIGHEST_AIN_NUMBER 7

static void checkOutOfBounds(const int AIN_NUMBER) {
    if (AIN_NUMBER < LOWEST_AIN_NUMBER || AIN_NUMBER > HIGHEST_AIN_NUMBER) {
        printf("Error: AIN%d does not exist\n", AIN_NUMBER);
        exit(1);
    }
}

int A2D_readVoltage(const int AIN_NUMBER)
{
    checkOutOfBounds(AIN_NUMBER);
    char path[MAX_SIZE];
    snprintf(path, MAX_SIZE, "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw", AIN_NUMBER);
    FILE *pFile = File_getFilePointer(path, "r", "in_voltage_raw");
    char *readData = File_extractFileContents(pFile);
    int voltageReading = atoi(readData);
    return voltageReading;
}