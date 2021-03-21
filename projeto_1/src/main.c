#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <modbus.h>
#include <bme280.h>
#include <i2c_bme.h>
#include <i2c_lcd.h>

void sig_handler(int signal){
    if (signal == SIGINT){
        close_UART();
        exit(0);
    }
}


int main(int argc, const char * argv[]) {
    const char INTERN_TEMPERATURE = 0xC1;
    const char POTENTIOMETER_TEMPERATURE = 0xC2;

    float tenv, tref, tin;

    struct identifier id;
    struct bme280_dev dev;

    signal(SIGINT, sig_handler);

    initialize_UART();

    setup_i2c_bme(&id, &dev);
    open_i2c_conn(&id, &dev);
    initialize_I2C(&dev);
    lcd_init();

    while(1){
        tin = read_temperature(INTERN_TEMPERATURE);
        printf("Intern temperature in main = %f\n", tin);

        tref = read_temperature(POTENTIOMETER_TEMPERATURE);
        printf("Reference temperature in main = %f\n", tref);


        read_temperature_i2c(&dev, &tenv);
        printf("Evironment temperature = %f\n", tenv);

        printf("Escrevendo no lcd...\n");
        write_LCD(tenv, tref, tin);

    }

    close_UART();

   return 0;
}
