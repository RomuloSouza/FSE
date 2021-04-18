#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void setup_gpio();
void toggle_switch(int pin);

#endif /* GPIO_H_ */
