#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

void setup_gpio();
void handle_temperature(int intensity);
void turn_off_gpio();

#endif /* GPIO_H_ */
