#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <csv.h>
// #include <gpio.h>
#include <bme280.h>
#include <i2c_bme.h>

void sig_handler(int signal){
    if (signal == SIGINT){
        // close_file();
        // turn_off_gpio();
        sleep(1);

        exit(0);
    }
}

int main(int argc, const char * argv[]) {
    // int should_write = 1; // assert the data will be written each 2s

    float te;

    struct identifier id;
    struct bme280_dev dev;

    // signal(SIGINT, sig_handler);

    // BME configuration
    setup_i2c_bme(&id, &dev);
    open_i2c_conn(&id, &dev);
    initialize_I2C(&dev);

    // GPIO configuration
    // setup_gpio();

    // Create CSV file
    // create_file();

    while(1){
        read_temperature_i2c(&dev, &te);
        printf("Evironment temperature = %f\n", te);

        // Write into log file
        // if(should_write){
        //     printf("Writing into log file...\n");
        //     should_write--;
        // } else {
        //     should_write++;
        // }

        usleep(800000);
    }

   return 0;
}
