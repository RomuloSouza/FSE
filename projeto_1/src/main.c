#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <crc16.h>
#include <string.h>
#include <modbus.h>

int main(int argc, const char * argv[]) {


    initialize_UART();
    float temperature = read_intern_temperature();
    printf("Temperature in main = %f\n", temperature);

    close_UART();

   return 0;
}
