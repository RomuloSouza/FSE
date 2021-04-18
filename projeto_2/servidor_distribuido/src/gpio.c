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

void _update_sensor_press_1();

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

States states;

void setup_gpio(){
    wiringPiSetup();

    // Lamps and air
    pinMode(LAMP_1, OUTPUT);
    pinMode(LAMP_2, OUTPUT);
    pinMode(LAMP_3, OUTPUT);
    pinMode(LAMP_4, OUTPUT);
    pinMode(AIR_1, OUTPUT);
    pinMode(AIR_2, OUTPUT);

    // Sensors
    pinMode(SENSOR_PRES_1, OUTPUT);
    pinMode(SENSOR_PRES_2, OUTPUT);
    pinMode(SENSOR_DOOR_KITCHEN, OUTPUT);
    pinMode(SENSOR_WINDOW_KITCHEN, OUTPUT);
    pinMode(SENSOR_DOOR_LIVING_ROOM, OUTPUT);
    pinMode(SENSOR_WINDOW_LIVING_ROOM, OUTPUT);
    pinMode(SENSOR_WINDOW_ROOM_1, OUTPUT);
    pinMode(SENSOR_WINDOW_ROOM_2, OUTPUT);
}

void setup_interrupts(){
    wiringPiISR(SENSOR_PRES_1, INT_EDGE_BOTH,  &_update_sensor_press_1);
}

void toggle_switch(int pin){
    int state = digitalRead(pin);

    if(state)
        digitalWrite(pin, LOW);
    else
        digitalWrite(pin, HIGH);

}


// ====================================== LOCAL FUNCTIONS ======================================

void _initializa_states(){
    states.lamp_1 = digitalRead(LAMP_1);
    states.lamp_2 = digitalRead(LAMP_2);
    states.lamp_3 = digitalRead(LAMP_3);
    states.lamp_4 = digitalRead(LAMP_4);
    states.sensor_door_kitchen = digitalRead(SENSOR_DOOR_KITCHEN);
    states.sensor_door_living_room = digitalRead(SENSOR_DOOR_LIVING_ROOM);
    states.sensor_pres_1 = digitalRead(SENSOR_PRES_1);
    states.sensor_pres_2 = digitalRead(SENSOR_PRES_2);
    states.sensor_window_kitchen = digitalRead(SENSOR_WINDOW_KITCHEN);
    states.sensor_window_living_room = digitalRead(SENSOR_WINDOW_LIVING_ROOM);
    states.sensor_window_room_1 = digitalRead(SENSOR_WINDOW_ROOM_1);
    states.sensor_window_room_2 = digitalRead(SENSOR_WINDOW_ROOM_2);
}

int _change_state(int state){
    return state ? 0 : 1;
}

void _update_sensor_press_1(){
    states.sensor_pres_1 = _change_state(states.sensor_pres_1);
}
