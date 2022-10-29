/* Used to initialize and display two digit numbers on the LED Matrix */

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

void LEDMatrix_initMatrix(void);
void LEDMatrix_displayInt(int number);
void LEDMatrix_displayFloat(float number);
void LEDMatrix_clearDisplay(void);

#endif