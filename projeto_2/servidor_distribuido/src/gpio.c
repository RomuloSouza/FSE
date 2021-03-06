#include <gpio.h>
#include <serializer.h>
#include <client.h>

// Variable shared between modules
States states;

void _update_sensor_door_kitchen();
void _update_sensor_door_living_room();

void _update_sensor_pres_1();
void _update_sensor_pres_2();

void _update_sensor_window_kitchen();
void _update_sensor_window_living_room();

void _update_sensor_window_room_1();
void _update_sensor_window_room_2();

void _send_message();
void _setup_interrupts();
void _initialize_states();
void _update_state(int pin);
int _change_state(int state);

int serialize_states(char *buff){
    int size = 0;

    serialize(buff, &size, "lamp_1:", states.lamp_1, "%d");
    serialize(buff, &size, "lamp_2:", states.lamp_2, "%d");
    serialize(buff, &size, "lamp_3:", states.lamp_3, "%d");
    serialize(buff, &size, "lamp_4:", states.lamp_4, "%d");
    serialize(buff, &size, "air_1:", states.air_1, "%d");
    serialize(buff, &size, "air_2:", states.air_2, "%d");
    serialize(buff, &size, "sensor_pres_1:", states.sensor_pres_1, "%d");
    serialize(buff, &size, "sensor_pres_2:", states.sensor_pres_2, "%d");
    serialize(buff, &size, "sensor_door_kitchen:", states.sensor_door_kitchen, "%d");
    serialize(buff, &size, "sensor_window_kitchen:", states.sensor_window_kitchen, "%d");
    serialize(buff, &size, "sensor_door_living_room:", states.sensor_door_living_room, "%d");
    serialize(buff, &size, "sensor_window_living_room:", states.sensor_window_living_room, "%d");
    serialize(buff, &size, "sensor_window_room_1:", states.sensor_window_room_1, "%d");
    serialize(buff, &size, "sensor_window_room_2:", states.sensor_window_room_2, "%d");

    return size;
}

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

    _setup_interrupts();
    _initialize_states();

}

void toggle_switch(int pin){
    int state = digitalRead(pin);

    if(state)
        digitalWrite(pin, LOW);
    else
        digitalWrite(pin, HIGH);

    _update_state(pin);
}

// ====================================== LOCAL FUNCTIONS ======================================

void _setup_interrupts(){
    wiringPiISR(SENSOR_PRES_1, INT_EDGE_BOTH,  &_update_sensor_pres_1);
    wiringPiISR(SENSOR_PRES_2, INT_EDGE_BOTH,  &_update_sensor_pres_2);
    wiringPiISR(SENSOR_DOOR_KITCHEN, INT_EDGE_BOTH,  &_update_sensor_door_kitchen);
    wiringPiISR(SENSOR_WINDOW_KITCHEN, INT_EDGE_BOTH,  &_update_sensor_window_kitchen);
    wiringPiISR(SENSOR_DOOR_LIVING_ROOM, INT_EDGE_BOTH,  &_update_sensor_door_living_room);
    wiringPiISR(SENSOR_WINDOW_LIVING_ROOM, INT_EDGE_BOTH,  &_update_sensor_window_living_room);
    wiringPiISR(SENSOR_WINDOW_ROOM_1, INT_EDGE_BOTH,  &_update_sensor_window_room_1);
    wiringPiISR(SENSOR_WINDOW_ROOM_2, INT_EDGE_BOTH,  &_update_sensor_window_room_2);
}

void _initialize_states(){
    states.lamp_1 = digitalRead(LAMP_1);
    states.lamp_2 = digitalRead(LAMP_2);
    states.lamp_3 = digitalRead(LAMP_3);
    states.lamp_4 = digitalRead(LAMP_4);
    states.air_1 = digitalRead(AIR_1);
    states.air_2 = digitalRead(AIR_2);
    states.sensor_door_kitchen = digitalRead(SENSOR_DOOR_KITCHEN);
    states.sensor_door_living_room = digitalRead(SENSOR_DOOR_LIVING_ROOM);
    states.sensor_pres_1 = digitalRead(SENSOR_PRES_1);
    states.sensor_pres_2 = digitalRead(SENSOR_PRES_2);
    states.sensor_window_kitchen = digitalRead(SENSOR_WINDOW_KITCHEN);
    states.sensor_window_living_room = digitalRead(SENSOR_WINDOW_LIVING_ROOM);
    states.sensor_window_room_1 = digitalRead(SENSOR_WINDOW_ROOM_1);
    states.sensor_window_room_2 = digitalRead(SENSOR_WINDOW_ROOM_2);
}

void _update_state(int pin){
    switch (pin) {
    case LAMP_1:
        states.lamp_1 = _change_state(states.lamp_1);
        break;
    case LAMP_2:
        states.lamp_2 = _change_state(states.lamp_2);
        break;
    case LAMP_3:
        states.lamp_3 = _change_state(states.lamp_3);
        break;
    case LAMP_4:
        states.lamp_4 = _change_state(states.lamp_4);
        break;
    case AIR_1:
        states.air_1 = _change_state(states.air_1);
        break;
    case AIR_2:
        states.air_2 = _change_state(states.air_2);
        break;

    default:
        break;
    }
}

int _change_state(int state){
    return state ? 0 : 1;
}

void _send_message(){
    char msg[512];

    serialize_states(msg);
    send_message(msg);
}

// Functions to handle an alteration in a sensor state

void _update_sensor_door_kitchen(){
    states.sensor_door_kitchen = _change_state(states.sensor_door_kitchen);
    _update_state(SENSOR_DOOR_KITCHEN);
    _send_message();
}
void _update_sensor_door_living_room(){
    states.sensor_door_living_room = _change_state(states.sensor_door_living_room);
    _update_state(SENSOR_DOOR_LIVING_ROOM);
    _send_message();
}

void _update_sensor_pres_1(){
    states.sensor_pres_1 = _change_state(states.sensor_pres_1);
    _update_state(SENSOR_PRES_1);
    _send_message();
}

void _update_sensor_pres_2(){
    states.sensor_pres_2 = _change_state(states.sensor_pres_2);
    _update_state(SENSOR_PRES_2);
    _send_message();
}

void _update_sensor_window_kitchen(){
    states.sensor_window_kitchen = _change_state(states.sensor_window_kitchen);
    _update_state(SENSOR_WINDOW_KITCHEN);
    _send_message();
}

void _update_sensor_window_living_room(){
    states.sensor_window_living_room = _change_state(states.sensor_window_living_room);
    _update_state(SENSOR_WINDOW_LIVING_ROOM);
    _send_message();
}

void _update_sensor_window_room_1(){
    states.sensor_window_room_1 = _change_state(states.sensor_window_room_1);
    _update_state(SENSOR_WINDOW_ROOM_1);
    _send_message();
}

void _update_sensor_window_room_2(){
    states.sensor_window_room_2 = _change_state(states.sensor_window_room_2);
    _update_state(SENSOR_WINDOW_ROOM_2);
    _send_message();
}
