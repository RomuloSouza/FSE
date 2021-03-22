#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <csv.h>
#include <pid.h>
#include <gpio.h>
#include <modbus.h>
#include <bme280.h>
#include <i2c_bme.h>
#include <i2c_lcd.h>

void sig_handler(int signal){
    if (signal == SIGINT){
        close_file();
        close_UART();
        turn_off_gpio();
        sleep(1);

        exit(0);
    }
}

int main(int argc, const char * argv[]) {
    const char INTERN_TEMPERATURE = 0xC1;
    const char POTENTIOMETER_TEMPERATURE = 0xC2;

    int should_write = 1; // assert the data will be written each 2s

    float ti, te, tr;

    double kp = 5.0, ki = 1.0, kd = 5.0;
    double control_pid=0;
    int intensity=0;

    struct identifier id;
    struct bme280_dev dev;

    signal(SIGINT, sig_handler);

    // UART configuration
    initialize_UART();

    // BME configuration
    setup_i2c_bme(&id, &dev);
    open_i2c_conn(&id, &dev);
    initialize_I2C(&dev);

    // GPIO configuration
    setup_gpio();

    // LCD configuration
    lcd_init();

    // PID configuration
    pid_configura_constantes(kp, ki, kd);

    // Create CSV file
    create_file();

    while(1){
        // Read temperatures
        tr = read_temperature(POTENTIOMETER_TEMPERATURE);
        printf("Reference temperature = %f\n", tr);

        ti = read_temperature(INTERN_TEMPERATURE);
        printf("Intern temperature = %f\n", ti);

        read_temperature_i2c(&dev, &te);
        printf("Evironment temperature = %f\n", te);

        // Calulate PID
        pid_atualiza_referencia(tr);
        control_pid = pid_controle((double)ti);
        printf("Control PID = %lf\n", control_pid);

        // Turn ON/OFF the FAN/Resistor
        intensity = control_pid;
        printf("Intensity = %d\n", intensity);
        handle_temperature(intensity);

        // Write in LCD
        printf("Writing in LCD...\n");
        write_LCD(te, tr, ti);

        // Write into log file
        if(should_write){
            printf("Writing into log file...\n");
            write_to_file(ti, te, tr, control_pid);
            should_write--;
        } else {
            should_write++;
        }

        usleep(800000);
    }

    close_UART();

   return 0;
}
