CXX = arm-linux-gnueabihf-gcc
CXXFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Werror -pthread
OBJ = light_sampler
TARGETS = main.c UserButton.c GPIO.c File.c Terminal.c A2D.c Joystick.c I2C.c LEDMatrix.c
DIR = $(HOME)/cmpt433/public/myApps/light_sampler

all:
	$(CXX) $(CXXFLAGS) -o $(OBJ) $(TARGETS)
	cp $(OBJ) $(DIR)

clean:

	rm -f $(OBJ)
