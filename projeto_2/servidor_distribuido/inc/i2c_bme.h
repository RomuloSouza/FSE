#ifndef I2C_BME_H
#define I2C_BME_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <bme280.h>

struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};

void setup_i2c_bme(struct identifier *id, struct bme280_dev *dev);
void open_i2c_conn(struct identifier *id, struct bme280_dev *dev);
void initialize_I2C(struct bme280_dev *dev);

int8_t read_temperature_i2c(struct bme280_dev *dev, float *temp);

#endif
