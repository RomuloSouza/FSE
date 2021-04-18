#include <gpio.h>

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

void setup_gpio(){
    wiringPiSetup();
    pinMode(LAMP_1, OUTPUT);
    pinMode(LAMP_2, OUTPUT);
    pinMode(LAMP_3, OUTPUT);
    pinMode(LAMP_4, OUTPUT);
    pinMode(AIR_1, OUTPUT);
    pinMode(AIR_2, OUTPUT);
}

void toggle_switch(){
    int pin = 0;
    int state = digitalRead(pin);

    if(state)
        digitalWrite(pin, LOW);
    else
        digitalWrite(pin, HIGH);


    printf("Lamp_1 state = %d\n", state);

}

void turn_off_gpio(){
    printf("Turning fan and resistor off...\n");
    printf("Done\n");
}
