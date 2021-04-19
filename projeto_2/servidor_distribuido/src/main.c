#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <csv.h>
#include <gpio.h>
#include <bme280.h>
#include <i2c_bme.h>

#include <server.h>
#include <pthread.h>

pthread_t server_thread;

void sig_handler(int signal){
    if (signal == SIGINT){
        // close_file();

        printf("Finalizando as threads...\n");
        stop_server();
        pthread_cancel(server_thread);
        sleep(1);

        exit(0);
    }
}

int main(int argc, const char * argv[]) {
    float te, humidity;

    struct identifier id;
    struct bme280_dev dev;

    signal(SIGINT, sig_handler);

    // BME configuration
    setup_i2c_bme(&id, &dev);
    open_i2c_conn(&id, &dev);
    initialize_I2C(&dev);

    // GPIO configuration
    setup_gpio(&states);

    // Start socket
    pthread_create(&server_thread, NULL, (void*)create_server, NULL);

    // Create CSV file
    // create_file();

    while(1){
        read_temperature_i2c(&dev, &te, &humidity);
        printf("Evironment temperature = %f\n", te);
        printf("Humidity = %f\n", humidity);

        // toggle_switch(0);

        sleep(2);
    }

   return 0;
}
