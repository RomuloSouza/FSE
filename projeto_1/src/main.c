#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <crc16.h>
#include <string.h>
#include <modbus.h>

#include <sys/types.h>

#include <i2c_bme.h>
#include <bme280.h>

#include <i2c_lcd.h>

int main(int argc, const char * argv[]) {
    const char INTERN_TEMPERATURE = 0xC1;
    const char POTENTIOMETER_TEMPERATURE = 0xC2;

    float tenv, tref, tin;

    initialize_UART();
    tin = read_temperature(INTERN_TEMPERATURE);
    printf("Intern temperature in main = %f\n", tin);

    tref = read_temperature(POTENTIOMETER_TEMPERATURE);
    printf("Reference temperature in main = %f\n", tref);

    struct identifier id;
    struct bme280_dev dev;

    open_i2c_conn(&id);
    initialize_I2C(&id, &dev);

    read_temperature_i2c(&dev, &tenv);

    printf("Evironment temperature = %f\n", tenv);

    // INIT LCD
    lcd_init();

    printf("Escrevendo no lcd...\n");
    write_LCD(tenv, tref, tin);


    close_UART();

   return 0;
}
