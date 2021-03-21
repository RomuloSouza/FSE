#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <modbus.h>
#include <bme280.h>
#include <i2c_bme.h>
#include <i2c_lcd.h>
#include <pid.h>
#include <gpio.h>

void sig_handler(int signal){
    if (signal == SIGINT){
        close_UART();
        turn_off_gpio();
        sleep(1);
        exit(0);
    }
}


int main(int argc, const char * argv[]) {
    const char INTERN_TEMPERATURE = 0xC1;
    const char POTENTIOMETER_TEMPERATURE = 0xC2;

    float tenv, tref, tin;

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

    while(1){
        tref = read_temperature(POTENTIOMETER_TEMPERATURE);
        printf("Reference temperature in main = %f\n", tref);

        tin = read_temperature(INTERN_TEMPERATURE);
        printf("Intern temperature in main = %f\n", tin);

        read_temperature_i2c(&dev, &tenv);
        printf("Evironment temperature = %f\n", tenv);

        pid_atualiza_referencia(tref);
        control_pid = pid_controle((double)tin);
        printf("Control PID = %lf\n", control_pid);

        intensity = control_pid;
        printf("Intensity = %d\n", intensity);
        handle_temperature(intensity);

        printf("Escrevendo no lcd...\n");
        write_LCD(tenv, tref, tin);

        sleep(1);
    }

    close_UART();

   return 0;
}
