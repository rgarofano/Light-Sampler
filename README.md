Light sampler is a multi-threaded C program that can detect the number of dips in light every second and print statistics about the samples used to determine the number of dips.

This program was written in a Linux (Debian) environment and cross-compiled to a BeagleBone Green board which is also debian based but with an ARM architecture.

Hardware Configuration:

BeagleBone Green Board - USB tethered to a host computer

Photoresitor - connected with a 10kohm resistor to form a voltage divider which is read via A2D on the board

Adafruit Miniature 8x8 Yellow-Green LED Matrix - Connected via I2C to the board

Adafruit Mini 2-Axis Analog Thumbstick - Connected via A2D to the board

Program Output:

The program computes and prints the following data to the console every second
  - Number of dips in light
  - Minimum, maximum, and average voltage reading from the photoresistor
  - Number of times the photoresitor was sampled
  - Minimum, maximum, and average time between samples

Additionally the LED matrix also displays some information
  - Joystick centered -> number of dips
  - Joystick 'UP' -> maximum voltage reading
  - Joystick 'DOWN' -> minimum voltage reading
  - Joystick 'LEFT' -> minimum time interval
  - Joystick 'RIGHT' -> maximum time interval
