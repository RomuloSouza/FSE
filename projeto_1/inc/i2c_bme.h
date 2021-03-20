#ifndef I2C_H
#define I2C_H

#include <bme280.h>

struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};


void open_i2c_conn(struct identifier *id);
void initialize_I2C(struct identifier *id, struct bme280_dev *dev);

void print_sensor_data(struct bme280_data *comp_data);
int8_t read_temperature_i2c(struct bme280_dev *dev, float *temp);

#endif