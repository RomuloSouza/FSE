#include <gpio.h>

#define RES_PIN 4
#define FAN_PIN 5

void setup_gpio(){

	wiringPiSetup();		    /* initialize wiringPi setup */
	pinMode(RES_PIN, OUTPUT);	/* set GPIO as output */
	softPwmCreate(RES_PIN, 1, 100); /* set PWM channel along with range*/

	pinMode(FAN_PIN, OUTPUT);
	softPwmCreate(FAN_PIN, 1, 100);
}

void handle_temperature(int intensity){
    if (intensity > 0){
        softPwmWrite (FAN_PIN, 0);
        softPwmWrite (RES_PIN, intensity);
    } else if (intensity < -40){
        softPwmWrite (FAN_PIN, intensity * -1);
        softPwmWrite (RES_PIN, 0);
    } else {
        softPwmWrite (RES_PIN, 0);
        softPwmWrite (FAN_PIN, 0);
    }
}


void turn_off_gpio(){
    printf("Turning fan and resistor off...\n");
    softPwmWrite (RES_PIN, 0);
    softPwmWrite (FAN_PIN, 0);
    printf("Done\n");
}
