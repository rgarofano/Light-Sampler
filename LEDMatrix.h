/* Used to initialize and display two digit numbers on the LED Matrix */

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

void LEDMatrix_initMatrix(void);
void LEDMatrix_startDisplay(void);
void LEDMatrix_updateDisplayValues(int numDips, int maxVoltage, int minVoltage, long long minTimeIntreval, long long maxTimeIntreval);
void LEDMatrix_stopRunning(void);

#endif