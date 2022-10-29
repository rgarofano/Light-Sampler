#include <stdio.h>
#include <string.h>
#include "File.h"
#include "Terminal.h"

#define MAX_SIZE 1024

typedef struct {
    char* bbgPinNumber;
    int linuxGPIONumber;
    char* directionPath;
    char* valuePath;
} gpioDevice_t;

static gpioDevice_t gpioDevices[] = {
    {"P8.43",
     72,
     "/sys/class/gpio/gpio72/direction",
     "/sys/class/gpio/gpio72/value"
    }
};

void GPIO_configurePin(const int INDEX)
{
    const char* PIN = gpioDevices[INDEX].bbgPinNumber;
    char command[MAX_SIZE];
    snprintf(command, MAX_SIZE, "config-pin %s gpio", PIN);
    Terminal_runCommand(command);
}

void GPIO_setInput(const int INDEX)
{
    FILE* pFile = File_getFilePointer(gpioDevices[INDEX].directionPath, "w", "direction");
    fprintf(pFile, "%s", "in");
    fclose(pFile);
}

char *GPIO_readValue(const int INDEX) {
    char* valuePath = gpioDevices[INDEX].valuePath;
    FILE* pFile = File_getFilePointer(valuePath, "r", "value");
    return File_extractFileContents(pFile);
}