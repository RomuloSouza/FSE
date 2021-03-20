#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <crc16.h>
#include <string.h>
#include <modbus.h>

#include <sys/types.h>

#include <i2c.h>
#include <bme280.h>

int main(int argc, const char * argv[]) {
    const char INTERN_TEMPERATURE = 0xC1;
    const char POTENTIOMETER_TEMPERATURE = 0xC2;

    initialize_UART();
    float temperature = read_temperature(INTERN_TEMPERATURE);
    printf("Intern temperature in main = %f\n", temperature);

    temperature = read_temperature(POTENTIOMETER_TEMPERATURE);
    printf("Reference temperature in main = %f\n", temperature);

    struct identifier id;
    struct bme280_dev dev;

    open_i2c_conn(&id);
    initialize_I2C(&id, &dev);

    float environ_temp=0;
    environ_temp = read_temperature_i2c(&dev, &environ_temp);

    printf("Evironment temperature = %f\n", environ_temp);


    close_UART();

   return 0;
}
