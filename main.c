#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include "UserButton.h"
#include "Joystick.h"
#include "LEDMatrix.h"

#define BUTTON_CHECK_DELAY_MS 100

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void* checkForButtonPress(void* arg)
{
    while(!UserButton_isButtonPressed()) {
        sleepForMs(BUTTON_CHECK_DELAY_MS);
    }
    
    return NULL;
}

int main(void) {

    /*** Initialize hardware ***/

    UserButton_initButton();
    LEDMatrix_initMatrix();
    
    /*** Create thread for the user button ***/
    
    // Thread ID
    pthread_t btnTid;

    // Create Attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&btnTid, &attr, checkForButtonPress, NULL);

    // Wait until the button thread is done executing
    pthread_join(btnTid, NULL);

    LEDMatrix_clearDisplay();

    return 0;
}