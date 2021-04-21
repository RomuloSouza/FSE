#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <gpio.h>
#include <bme280.h>
#include <i2c_bme.h>

#include <server.h>
#include <client.h>
#include <serializer.h>

struct identifier id;
struct bme280_dev dev;

int is_running = 1;

void sig_alarm(int signal){
    float temperature, humidity;

    read_temperature_i2c(&dev, &temperature, &humidity);
    printf("Evironment temperature = %f\n", temperature);
    printf("Humidity = %f\n", humidity);

    char serialized_temperature[MAX_BUFFER_SIZE];
    serialize_temperature(serialized_temperature, temperature, humidity);
    send_message(serialized_temperature);

    if (is_running){
        alarm(1);
    }
}

void sig_stop(int signal){
    if (signal == SIGINT){
        printf("Closing sockets...\n");
        is_running = 0;
        usleep(1100000); // Awaits the last alarm to execute

        stop_server();
        close_client_socket();
        usleep(500000);

        exit(0);
    }
}

int main(int argc, const char * argv[]) {

    signal(SIGINT, sig_stop);
    signal(SIGALRM, sig_alarm);

    // BME configuration
    setup_i2c_bme(&id, &dev);
    open_i2c_conn(&id, &dev);
    initialize_I2C(&dev);

    // Schedule alarm to read temperature
    alarm(1);

    // GPIO configuration
    setup_gpio(&states);

    // Start client socket
    open_client_socket();

    create_server();

   return 0;
}
