#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"

#define NUM_BYTES 8
#define BUFF_SIZE 16
#define LED_MATRIX_IDX 0
#define LED_SETUP_REG 0x21
#define DISPLAY_SETUP_REG 0x81
#define DISPLAY_REG 0x00
#define WRITE_TURN_ON 0x00
#define FIRST_DIGIT_MASK 0b01110000
#define SECOND_DIGIT_MASK 0b10000011
#define FLOATING_POINT 0b00000100

typedef struct {
    unsigned char bytes[NUM_BYTES];
} bitPattern_t;

static bitPattern_t digits[10] = {
    // row   0x00  0x02  0x04  0x06  0x08  0x0A  0x0C  0x0E
    /* 0 */ {{0x00, 0xF3, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xF3}},
    /* 1 */ {{0x00, 0xF3, 0x21, 0x21, 0x21, 0x21, 0x63, 0x21}},
    /* 2 */ {{0x00, 0xF3, 0x42, 0x63, 0xB1, 0x90, 0xD2, 0xF3}},
    /* 3 */ {{0x00, 0xF3, 0x90, 0x90, 0xF3, 0x90, 0x90, 0xF3}},
    /* 4 */ {{0x00, 0x90, 0x90, 0x90, 0xF3, 0xD2, 0xD2, 0x90}},
    /* 5 */ {{0x00, 0xF3, 0x90, 0x90, 0xF3, 0x42, 0x42, 0xF3}},
    /* 6 */ {{0x00, 0xF3, 0xD2, 0xD2, 0xF3, 0x42, 0x42, 0xF3}},
    /* 7 */ {{0x00, 0x21, 0x21, 0x21, 0x90, 0x90, 0x90, 0xF3}},
    /* 8 */ {{0x00, 0xF3, 0xD2, 0xD2, 0xF3, 0xD2, 0xD2, 0xF3}},
    /* 9 */ {{0x00, 0xF3, 0x90, 0x90, 0xF3, 0xD2, 0xD2, 0xF3}}
};

void LEDMatrix_initMatrix(void)
{
    I2C_configurePins(LED_MATRIX_IDX);
    I2C_initI2CBus(LED_MATRIX_IDX);
    I2C_writeByteToI2CReg(LED_MATRIX_IDX, LED_SETUP_REG, WRITE_TURN_ON);
    I2C_writeByteToI2CReg(LED_MATRIX_IDX, DISPLAY_SETUP_REG, WRITE_TURN_ON);
}

static void checkIntegerBounds(int number)
{
    if (number < 0 || number > 99) {
        printf("Error: unable to display %d, only 2 digit non-negative numbers are supported.\n", number);
    }
}

static void displayDigits(unsigned char* buff, unsigned char* firstDigitBytes, unsigned char* secondDigitBytes)
{
    for (int i = 1; i < NUM_BYTES; i++) {
        unsigned char firstDigitBits = firstDigitBytes[i];
        unsigned char secondDigitBits = secondDigitBytes[i];
        firstDigitBits &= FIRST_DIGIT_MASK;
        secondDigitBits &= SECOND_DIGIT_MASK;
        buff[i * 2] = firstDigitBits + secondDigitBits;
    }

    // write the bytes to the I2C register to display them
    I2C_writeBytesToI2CReg(LED_MATRIX_IDX, DISPLAY_REG, buff, BUFF_SIZE);
}

void LEDMatrix_displayInt(int number) 
{
    // error handling
    checkIntegerBounds(number);

    // determine which digits need to be displayed and get the bit patterns for them
    const int FIRST_DIGIT = number / 10;
    const int SECOND_DIGIT = number % 10;
    unsigned char* firstDigitBytes = {0x00};
    if (FIRST_DIGIT != 0) {
        firstDigitBytes = digits[FIRST_DIGIT].bytes;   
    }
    unsigned char* secondDigitBytes = digits[SECOND_DIGIT].bytes;

    // initialize byte array for the I2C register
    unsigned char buff[BUFF_SIZE] = {};
    displayDigits(buff, firstDigitBytes, secondDigitBytes);
}

static void checkFloatBounds(float number) {
    if (number < 0 || number >= 10) {
        printf("Error: unable to display %f, only 2 digit non-negative numbers are supported.\n", number);
    }
}

void LEDMatrix_displayFloat(float number)
{
    // error handling
    checkFloatBounds(number);

    // determine which digits need to be displayed and get the bit patterns for them
    const int FIRST_DIGIT = (int) number;
    const int SECOND_DIGIT = (int) ((number - FIRST_DIGIT) * 10);
    unsigned char* firstDigitBytes = digits[FIRST_DIGIT].bytes;
    unsigned char* secondDigitBytes = digits[SECOND_DIGIT].bytes;

    // initialize byte array for the I2C register
    unsigned char buff[BUFF_SIZE] = {FLOATING_POINT};
    displayDigits(buff, firstDigitBytes, secondDigitBytes);
}

void LEDMatrix_clearDisplay(void)
{
    unsigned char buff[BUFF_SIZE] = {0x00};
    I2C_writeBytesToI2CReg(LED_MATRIX_IDX, DISPLAY_REG, buff, BUFF_SIZE);
}