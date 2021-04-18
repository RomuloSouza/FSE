#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define LAMP_1 0                      // KITCHEN
#define LAMP_2 1                      // LIVING ROOM
#define LAMP_3 2                      // ROOM 1
#define LAMP_4 3                      // ROOM 2
#define AIR_1 23                      // ROOM 1
#define AIR_2 24                      // ROOM 2
#define SENSOR_PRES_1 6               // LIVING ROOM
#define SENSOR_PRES_2 25              // KITCHEN
#define SENSOR_DOOR_KITCHEN 21        // KITCHEN'S DOOR
#define SENSOR_WINDOW_KITCHEN 22      // KITCHEN'S WINDOW
#define SENSOR_DOOR_LIVING_ROOM 26    // LIVING ROOM'S DOOR
#define SENSOR_WINDOW_LIVING_ROOM 27  // LIVING ROOM'S WINDOW
#define SENSOR_WINDOW_ROOM_1 28       // ROOM_1'S WINDOW
#define SENSOR_WINDOW_ROOM_2 29       // ROOM_2'S WINDOW

typedef struct States {
    int lamp_1;
    int lamp_2;
    int lamp_3;
    int lamp_4;
    int air_1;
    int air_2;
    int sensor_pres_1;
    int sensor_pres_2;
    int sensor_door_kitchen;
    int sensor_window_kitchen;
    int sensor_door_living_room;
    int sensor_window_living_room;
    int sensor_window_room_1;
    int sensor_window_room_2;
} States;

void setup_gpio();
void toggle_switch(int pin);

#endif /* GPIO_H_ */
