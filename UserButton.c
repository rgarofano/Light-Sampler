#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "GPIO.h"

#define MAX_SIZE 1024
#define USR_BUTTON_IDX 0

void UserButton_initButton(void)
{
    GPIO_configurePin(USR_BUTTON_IDX);
    GPIO_setInput(USR_BUTTON_IDX);
}

bool UserButton_isButtonPressed(void)
{
    char* readData = GPIO_readValue(USR_BUTTON_IDX);
    bool isPressed = strncmp(readData, "1", strlen("1"));
    free(readData);
    readData = NULL;
    return isPressed;
}