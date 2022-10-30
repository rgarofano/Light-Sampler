#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "UserButton.h"
#include "LEDMatrix.h"
#include "Sleep.h"
#include "Sampler.h"

#define BUTTON_CHECK_DELAY_MS 100

void waitForUserButtonPress(void)
{
    while(!UserButton_isButtonPressed()) {
        Sleep_waitForMs(BUTTON_CHECK_DELAY_MS);
    }
}

void safeShutdown(void)
{
    Sampler_stopSampling();
    LEDMatrix_stopRunning();
}

int main(void) {

    // initialize hardware
    UserButton_initButton();
    LEDMatrix_initMatrix();
    
    // // Start the sampling threads
    Sampler_startSampling();

    // // Initiate real time display on the LED Matrix
    LEDMatrix_startDisplay();


    waitForUserButtonPress();

    // end the program safely
    safeShutdown();

    return 0;
}