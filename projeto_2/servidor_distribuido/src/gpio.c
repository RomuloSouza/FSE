#include <gpio.h>
#include <serializer.h>

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

void _initialize_states();

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

    printf("size value = %d\n", size);
    printf("buff len = %d\n", strlen(buff));

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

    _initialize_states();

}

void setup_interrupts(){
    wiringPiISR(SENSOR_PRES_1, INT_EDGE_BOTH,  &_update_sensor_pres_1);
}

void toggle_switch(int pin){
    int state = digitalRead(pin);

    if(state)
        digitalWrite(pin, LOW);
    else
        digitalWrite(pin, HIGH);

}


// ====================================== LOCAL FUNCTIONS ======================================

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

int _change_state(int state){
    return state ? 0 : 1;
}

void _update_sensor_door_kitchen(){
    states.sensor_door_kitchen = _change_state(states.sensor_door_kitchen);
}
void _update_sensor_door_living_room(){
    states.sensor_door_living_room = _change_state(states.sensor_door_living_room);
}

void _update_sensor_pres_1(){
    printf("state sensor pres_1 antes = %d\n", states.sensor_pres_1);
    states.sensor_pres_1 = _change_state(states.sensor_pres_1);
    printf("state sensor pres_1 dps = %d\n", states.sensor_pres_1);
}
void _update_sensor_pres_2(){
    states.sensor_pres_2 = _change_state(states.sensor_pres_2);
}

void _update_sensor_window_kitchen(){
    states.sensor_window_kitchen = _change_state(states.sensor_window_kitchen);
}
void _update_sensor_window_living_room(){
    states.sensor_window_living_room = _change_state(states.sensor_window_living_room);
}

void _update_sensor_window_room_1(){
    states.sensor_window_room_1 = _change_state(states.sensor_window_room_1);
}
void _update_sensor_window_room_2(){
    states.sensor_window_room_2 = _change_state(states.sensor_window_room_2);
}
