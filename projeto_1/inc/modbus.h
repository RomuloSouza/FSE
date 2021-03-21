#ifndef MODBUS_H_
#define MODBUS_H_

#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <crc16.h>
#include <string.h>

void initialize_UART();
void close_UART();
float read_temperature(char code);

#endif