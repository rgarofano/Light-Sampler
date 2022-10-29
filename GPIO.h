/* Used to initialize a GPIO device and read its value */

#ifndef GPIO_H
#define GPIO_H

void  GPIO_configurePin(const int INDEX);
void  GPIO_setInput(const int INDEX);
char* GPIO_readValue(const int INDEX);

#endif