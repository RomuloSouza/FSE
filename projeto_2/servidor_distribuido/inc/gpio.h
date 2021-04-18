#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void setup_gpio();
void turn_off_gpio();
void toggle_switch();

#endif /* GPIO_H_ */
